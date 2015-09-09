# node_xslt

## Installation
```
npm install node_xslt --save
```

**Or:**

In the root directory, run `node-gyp rebuild` to generate `./build/Release/node_xslt.node`

Then:

```JS
var xslt = require('node_xslt')
```

## Requirements

* [libxml2](http://www.xmlsoft.org/) (libxml2-dev package for Debian-based distros)
* [libxslt](http://xmlsoft.org/xslt/index.html) (libxslt-dev package for Debian-based distros)
* [libexslt](http://xmlsoft.org/xslt/EXSLT/) (libxslt-dev package for Debian-based distros)
* xml2-config (Needs to be on PATH)

## API

| Method | Arguments | Return | Description
| :---: | :---: | :---: | ---
| `readXsltString` | `string` | `Stylesheet {}` | Uses the specified XSLT for transformation
| `readXsltFile` | `string` | `Stylesheet {}` | Loads the XSLT from file
| `readXmlString`, `readHtmlString` | `string` | `Document {}` | Imports XML/HTML from string
| `readXmlFile`, `readHtmlFile` | string | `Document {}` | Imports the XML/HTML file
| `transform` | `Stylesheet`, `Document`, `string[]` | **string** | Transforms the document


### Example
```JS
xslt.transform(
  stylesheet,
  document,
  [
    'paramName',
    'paramValue',
    'paramName',
    'paramValue'
  ]
)
```
