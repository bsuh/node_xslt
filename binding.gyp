{
    "targets": [{
        "target_name": "node_xslt",
        "sources": [ "node_xslt.cc" ],
        "cflags": ["<!(xml2-config --cflags)", "-fexceptions -w"],
        "cflags_cc": ["<!(xml2-config --cflags)", "-fexceptions -w"],
        "xcode_settings": {
            "OTHER_CFLAGS": ["<!(xml2-config --cflags)", "-fexceptions"]
        },
        "libraries": ["-lxml2" , "-lxslt" , "-lexslt"],
        "library_dirs": [ "/usr/lib" ]
    }]
}
