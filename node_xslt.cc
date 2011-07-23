
#include <v8.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <string.h>
#include "scopeguard.h"

using namespace v8;

Handle<Value> err(const char * msg) {
    return ThrowException(Exception::Error(String::New(msg)));
}

xmlDocPtr xmlReadMemory(const char * buffer, int size) {
    xmlDocPtr doc = xmlReadMemory(buffer, size, NULL, "UTF-8", 0);
    if (!doc) {
        throw err("Failed to parse XML");
    }
    return doc;
}

xsltStylesheetPtr xsltReadMemory(const char * buffer, int size) {
    xmlDocPtr doc = xmlReadMemory(buffer, size, NULL, "UTF-8", 0);
    if (!doc) {
        throw err("Failed to parse XML");
    }
    ScopeGuard guard =  MakeGuard(xmlFreeDoc, doc);

    xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc(doc);
    if (!stylesheet) {
        throw err("Failed to parse stylesheet"); 
    }
    guard.Dismiss();
    return stylesheet;
}

xmlDocPtr xsltTransform(xsltStylesheetPtr stylesheet, xmlDocPtr doc, const char **params) {
    xmlDocPtr result = xsltApplyStylesheet(stylesheet, doc, params);
    if (!result) {
        throw err("Failed to apply stylesheet");
    }
    return result;
}

xmlOutputBufferPtr xmlAllocOutputBuffer() {
    xmlOutputBufferPtr outbuf = xmlAllocOutputBuffer(xmlGetCharEncodingHandler(XML_CHAR_ENCODING_UTF8));
    if (!outbuf) {
        throw err("Failed to allocate XML output buffer");
    }
    return outbuf;
}

void freeArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

Handle<Value> transform(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 3) {
        return err("Must supply at least 3 arguments");
    }

    if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsArray()) {
        return err("Must supply 2 strings and an array");
    }

    String::Utf8Value utf8Stylesheet(args[0]);
    String::Utf8Value utf8Xml(args[1]);
    Local<Array> array = Array::Cast(*args[2]);

    if (array->Length() % 2 != 0) {
        return err("Array contains an odd number of parameters");
    }

    char** params = (char **)malloc(sizeof(char *) * (array->Length() + 1));
    if (!params) {
        return err("Failed to allocate memory");
    }
    memset(params, 0, sizeof(char *) * (array->Length() + 1));
    ON_BLOCK_EXIT(freeArray, params, array->Length());

    for (int i = 0; i < array->Length(); i++) {
        Local<String> param = array->Get(Number::New(i))->ToString();
        params[i] = (char *)malloc(sizeof(char) * (param->Length() + 1));
        if (!params[i]) {
            return err("Failed to allocate memory");
        }
        param->WriteAscii(params[i]);
    }

    try {
        xsltStylesheetPtr stylesheet = xsltReadMemory(*utf8Stylesheet, utf8Stylesheet.length());
        ON_BLOCK_EXIT(xsltFreeStylesheet, stylesheet);

        xmlDocPtr docXml = xmlReadMemory(*utf8Xml, utf8Xml.length());
        ON_BLOCK_EXIT(xmlFreeDoc, docXml);

        xmlDocPtr result = xsltTransform(stylesheet, docXml, (const char **)params);
        ON_BLOCK_EXIT(xmlFreeDoc, result);

        xmlChar *doc_ptr;
        int doc_len;
        xsltSaveResultToString(&doc_ptr, &doc_len, result, stylesheet);
        ON_BLOCK_EXIT(xmlFree, doc_ptr);

        return scope.Close(String::New((const char *)doc_ptr, doc_len));
    } catch (Handle<Value> err) {
        return err;
    }
}

extern "C" void init(Handle<Object> target)
{
    HandleScope scope;

    target->Set(String::NewSymbol("transform"), FunctionTemplate::New(transform)->GetFunction());
}
