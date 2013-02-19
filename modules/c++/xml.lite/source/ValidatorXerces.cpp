/* =========================================================================
 * This file is part of xml.lite-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2011, General Dynamics - Advanced Information Systems
 *
 * xml.lite-c++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this program; If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 */
#ifdef USE_XERCES

#include "xml/lite/ValidatorXerces.h"
#include <sys/OS.h>
#include <io/ByteStream.h>
#include <mem/ScopedArray.h>

using namespace xml::lite;

bool xml::lite::ValidationErrorHandler::handleError(
        const ValidationError& err)
{
    std::string level;
    switch(err.getSeverity())
    {
    case xercesc::DOMError::DOM_SEVERITY_WARNING : 
        level = "WARNING"; 
        break;
    case xercesc::DOMError::DOM_SEVERITY_ERROR : 
        level = "ERROR"; 
        break;
    case xercesc::DOMError::DOM_SEVERITY_FATAL_ERROR : 
        level = "FATAL"; 
        break;
    }

    // transcode the file and message
    char* fChar = xercesc::XMLString::transcode(
                            err.getLocation()->getURI());
    char* mChar = xercesc::XMLString::transcode(
                            err.getMessage());

    std::string file = (fChar) ? fChar : "";
    std::string message = (mChar) ? mChar : "";

    // clean up
    xercesc::XMLString::release(&fChar);
    xercesc::XMLString::release(&mChar);

    // create o
    xml::lite::ValidationInfo info (
        message, level, file, 
        (size_t)err.getLocation()->getLineNumber());
    mErrorLog.push_back(info);

    return true;
}


ValidatorXerces::ValidatorXerces(
    const std::vector<std::string>& schemaPaths, bool recursive)
{
    // add each schema into a grammar pool --
    // this allows reuse
    mSchemaPool.reset(
        new xercesc::XMLGrammarPoolImpl(
            xercesc::XMLPlatformUtils::fgMemoryManager));

    const XMLCh ls_id [] = {xercesc::chLatin_L, 
                            xercesc::chLatin_S, 
                            xercesc::chNull};

    // create the validator
    mValidator.reset(
        xercesc::DOMImplementationRegistry::
            getDOMImplementation (ls_id)->createLSParser(
                xercesc::DOMImplementationLS::MODE_SYNCHRONOUS,
                0, 
                xercesc::XMLPlatformUtils::fgMemoryManager,
                mSchemaPool.get()));

    // set the configuration settings
    xercesc::DOMConfiguration* config = mValidator->getDomConfig();
    config->setParameter(xercesc::XMLUni::fgDOMComments, false);
    config->setParameter(xercesc::XMLUni::fgDOMDatatypeNormalization, true);
    config->setParameter(xercesc::XMLUni::fgDOMEntities, false);
    config->setParameter(xercesc::XMLUni::fgDOMNamespaces, true);
    config->setParameter(xercesc::XMLUni::fgDOMElementContentWhitespace, false);

    // validation settings
    config->setParameter(xercesc::XMLUni::fgDOMValidate, true);
    config->setParameter(xercesc::XMLUni::fgXercesSchema, true);
    config->setParameter(xercesc::XMLUni::fgXercesSchemaFullChecking, false); // this affects performance

    // definitely use cache grammer -- this is the cached schema
    config->setParameter(xercesc::XMLUni::fgXercesUseCachedGrammarInParse, true);

    // explicitly skip loading schema referenced in the xml docs
    config->setParameter(xercesc::XMLUni::fgXercesLoadSchema, false);

    // load additional schema referenced within schemas
    config->setParameter(xercesc::XMLUni::fgXercesHandleMultipleImports, true);

    // it's up to the user to clear the cached schemas
    config->setParameter(xercesc::XMLUni::fgXercesUserAdoptsDOMDocument, true);

    // add a error handler we still have control over
    mErrorHandler.reset(
            new xml::lite::ValidationErrorHandler());
    config->setParameter(xercesc::XMLUni::fgDOMErrorHandler, 
                         mErrorHandler.get());

    // load our schemas --
    // search each directory for schemas
    sys::OS os;
    std::vector<std::string> schemas = 
        os.search(schemaPaths, "", ".xsd", recursive);

    //  add the schema to the validator
    for (size_t i = 0; i < schemas.size(); ++i)
    {
        if (!mValidator->loadGrammar(schemas[i].c_str(), 
                                     xercesc::Grammar::SchemaGrammarType,
                                     true))
        {
            std::cout << "Error: Failure to load schema " << 
                schemas[i] << std::endl;
        }
    }

    //! no additional schemas will be loaded after this point!
    mSchemaPool->lockPool();
}

ValidatorXerces::~ValidatorXerces()
{
    
}

bool ValidatorXerces::validate(std::vector<ValidationInfo>& errors,
                               io::InputStream& xml,
                               sys::SSize_T size)
{
    // clear the log before its use -- 
    // however we do not clear the users 'errors' because 
    // they might want an accumulation of errors
    mErrorHandler->clearErrorLog();

    // get a vehicle to validate data
    xercesc::DOMLSInputImpl input(
        xercesc::XMLPlatformUtils::fgMemoryManager);

    // expand to the wide character data for use with xerces
    io::ByteStream bs;
    xml.streamTo(bs);

    input.setStringData(
        xercesc::XMLString::transcode(
            bs.stream().str().c_str()));

    // validate the document
    mValidator->parse(&input);

    // add the new errors to the vector 
    errors.insert(errors.end(), 
                  mErrorHandler->getErrorLog().begin(), 
                  mErrorHandler->getErrorLog().end());

    return (mErrorHandler->getErrorLog().size() > 0);
}

#endif
