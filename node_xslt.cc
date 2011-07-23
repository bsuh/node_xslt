
#include <v8.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <string.h>
#include "macros.h"
#include "scopeguard.h"

#define OBJ_DESTRUCTOR(d) Persistent<Object> _weak_handle = Persistent<Object>::New(self); \
                          _weak_handle.MakeWeak(NULL, d);

using namespace v8;

void jsXmlDocCleanup(Persistent<Value> value, void *) {
    HandleScope handlescope;
    Local<Object> obj = value->ToObject();
    EXTERNAL(xmlDocPtr, doc, obj, 0);
    xmlFreeDoc(doc);
    return;
}

void jsXsltStylesheetCleanup(Persistent<Value> value, void *) {
    HandleScope handlescope;
    Local<Object> obj = value->ToObject();
    EXTERNAL(xsltStylesheetPtr, style, obj, 0);
    xsltFreeStylesheet(style);
    return;
}

OBJECT(jsXmlDoc, 1, xmlDocPtr doc)
    INTERNAL(0, doc)
    OBJ_DESTRUCTOR(&jsXmlDocCleanup)
    RETURN_SCOPED(self);
END

OBJECT(jsXsltStylesheet, 1, xsltStylesheetPtr style)
    INTERNAL(0, style)
    OBJ_DESTRUCTOR(&jsXsltStylesheetCleanup)
    RETURN_SCOPED(self);
END

FUNCTION(readXmlString)
    ARG_COUNT(1)
    ARG_utf8(str, 0)

    xmlDocPtr doc = xmlReadMemory(*str, str.length(), NULL, "UTF-8", 0);
    if (!doc) {
        throw JS_ERROR("Failed to parse XML");
    }
    RETURN_SCOPED(jsXmlDoc(doc));
END

FUNCTION(readXsltString)
    ARG_COUNT(1)
    ARG_utf8(str, 0)

    xmlDocPtr doc = xmlReadMemory(*str, str.length(), NULL, "UTF-8", 0);
    if (!doc) {
        throw JS_ERROR("Failed to parse XML");
    }
    ScopeGuard guard =  MakeGuard(xmlFreeDoc, doc);

    xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc(doc);
    if (!stylesheet) {
        throw JS_ERROR("Failed to parse stylesheet"); 
    }
    guard.Dismiss();
    RETURN_SCOPED(jsXsltStylesheet(stylesheet));
END

void freeArray(char **array, int size) {
    for (int i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

FUNCTION(transform)
    ARG_COUNT(3)
    ARG_obj(objStylesheet, 0)
    ARG_obj(objDocument, 1)
    ARG_array(array, 2)

    EXTERNAL(xsltStylesheetPtr, stylesheet, objStylesheet, 0);
    EXTERNAL(xmlDocPtr, document, objDocument, 0);

    uint32_t arrayLen = array->Length();

    if (arrayLen % 2 != 0) {
        return JS_ERROR("Array contains an odd number of parameters");
    }

    char** params = (char **)malloc(sizeof(char *) * (arrayLen + 1));
    if (!params) {
        return JS_ERROR("Failed to allocate memory");
    }
    memset(params, 0, sizeof(char *) * (array->Length() + 1));
    ON_BLOCK_EXIT(freeArray, params, array->Length());

    for (int i = 0; i < array->Length(); i++) {
        Local<String> param = array->Get(JS_int(i))->ToString();
        params[i] = (char *)malloc(sizeof(char) * (param->Length() + 1));
        if (!params[i]) {
            return JS_ERROR("Failed to allocate memory");
        }
        param->WriteAscii(params[i]);
    }

    try {
        xmlDocPtr result = xsltApplyStylesheet(stylesheet, document, (const char **)params);
        if (!result) {
            throw JS_ERROR("Failed to apply stylesheet");
        }
        ON_BLOCK_EXIT(xmlFreeDoc, result);

        xmlChar *doc_ptr;
        int doc_len;
        xsltSaveResultToString(&doc_ptr, &doc_len, result, stylesheet);
        ON_BLOCK_EXIT(xmlFree, doc_ptr);

        RETURN_SCOPED(JS_str2((const char *)doc_ptr, doc_len));
    } catch (Handle<Value> err) {
        return err;
    }
END

extern "C" void init(Handle<Object> target)
{
    HandleScope scope;

    Handle<Object> self = target;
    BIND("readXmlString", readXmlString);
    BIND("readXsltString", readXsltString);
    BIND("transform", transform);
}
