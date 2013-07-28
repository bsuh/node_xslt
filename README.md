# node_xslt

    xslt = require('node_xslt')

    stylesheet = xslt.readXsltString(string);
    // readXsltString
    // Arguments: string containing XSLT
    // Returns: stylesheet object

    stylesheet = xslt.readXsltFile(filename);
    // readXsltFile
    // Arguments: filename to file containing XSLT
    // Returns: stylesheet object

    document = xslt.readXmlString(string);
    // readXmlString
    // Arguments: string containing XML
    // Returns: document object

    document = xslt.readXmlFile(filename);
    // readXmlFile
    // Arguments: filename to file containing XML
    // Returns: document object

    htmlDocument = xslt.readHtmlString(string);
    // readHtmlString
    // Arguments: string containing HTML
    // Returns: document object

    htmlDocument = xslt.readHtmlFile(string);
    // readHtmlFile
    // Arguments: filename to file containing HTML
    // Returns: document object
    
    transformedString = xslt.transform(stylesheet, document, parameters);
    // transform
    // Arguments:
    //  * stylesheet: stylesheet object
    //  * document: document object
    //  * parameters: an array of parameters to be passed to the stylesheet. length must be multiple of 2.
    //        Example: ['param1Name', 'param1Value', 'param2Name', 'param2Value']

## Requirements

* [libxml2](http://www.xmlsoft.org/) (libxml2-dev package for Debian-based distros)
* [libxslt](http://xmlsoft.org/xslt/index.html) (libxslt-dev package for Debian-based distros)
* [libexslt](http://xmlsoft.org/xslt/EXSLT/) (libxslt-dev package for Debian-based distros)
* xml2-config (Needs to be on PATH)

## Installation

**npm**

    npm install node_xslt

**source**

    In the root directory, run `node-gyp rebuild` to generate
    ./build/Release/node_xslt.node
