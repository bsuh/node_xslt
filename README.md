# node_xslt

    xslt = require('node_xslt')
    fs = require('fs')
    
    fs.readFile('test.xsl', function (err, xsltString) {
        if (err) throw err;

        stylesheet = xslt.readXsltString(xsltString);

        fs.readFile('cdcatalog.xml', function (err, xmlString) {
            if (err) throw err;

            doc = xslt.readXmlString(xmlString);
            transformedString = xslt.transform(stylesheet, doc, ['param1Name', 'param1Value', 'param2Name', 'param2Value']);
            console.log(transformedString);
        });

        fs.readFile('cdcatalog.html', function (err, htmlString) {
            ir (err) throw err;

            doc = xslt.readHtmlString(htmlString);
            transformedString = xslt.transform(stylesheet, doc, []);
            console.log(transformedString);
        });
    });

## Requirements

* [libxml2](http://www.xmlsoft.org/) (libxml2-dev package for Debian-based distros)
* [libxslt](http://www.xmlsoft.org/) (libxslt-dev package for Debian-based distros)
* xml2-config (Needs to be on PATH)
* node-waf & v8.h (Both come with node)

## Installation

**npm**

    npm install node_xslt

**source**

    In the root directory, run `node-waf configure build` to generate
    ./build/default/node_xslt.node
