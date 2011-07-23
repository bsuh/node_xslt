# node_xslt

    xslt = require('node_xslt')
    stylesheet = xslt.readXsltString(stylesheetString)
    document = xslt.readXmlString(documentString)
    xmlString = xslt.transform(stylesheet, document,
            ['paramName', 'paramValue', 'paramName2', 5])

## Requirements

* [libxml2](http://www.xmlsoft.org/)
* [libxslt](http://www.xmlsoft.org/)
* xml2-config (Needs to be on PATH)
* node-waf & v8.h (Both come with node)

## Installation

**npm**
    npm install node_xslt

**source**
    In the root directory, run `node-waf configure build` to generate
    ./build/default/node_xslt.node
