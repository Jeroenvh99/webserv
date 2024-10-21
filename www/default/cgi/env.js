#! /usr/local/bin/node

const http = require('http');
const os = require('os');
const process = require('process');

function element(tag, ...nested) {
    let body = "<" + tag + ">";
    for (const e of nested) {
        body += e;
    }
    body += "</" + tag + ">";
    return body;
}

function environValue(key) {
    const value = process.env[key];
    if (value === undefined) {
        return "&lt;undefined&gt;";
    } else if (value === "") {
        return "&lt;null&gt;";
    }
    return value;
}

function getPayload() {
    let html = "<!DOCTYPE html>";
    html += element("html",
        element("head",
            element("title", "CGI Execution Environment: JavaScript edition"),
            "<meta charset=\"utf-8\"> \
            <link rel=\"stylesheet\" href=\"/nested.css\">"
        ),
        element("body",
            element("h1", "CGI: Execution Environment: JavaScript edition"),
            element("p", "Directory: ", element("code", process.cwd())),
            element("ul",
                element("li", "Your IP address: ", element("code", environValue('REMOTE_HOST'))),
                element("li", "User agent: ", element("code", environValue('HTTP_USER_AGENT'))),
                element("li", "Request method: ", element("code", environValue('REQUEST_METHOD'))),
                element("li", "Server software: ", element("code", environValue("SERVER_SOFTWARE"))),
                element("li", "Server protocol: ", element("code", environValue("SERVER_PROTOCOL"))),
                element("li", "Server port: ", element("code", environValue("SERVER_PORT")))
            )
        )
    );
    return html;
}

const payload = getPayload();

console.log('Content-Type: text/html');
console.log(`Content-Length: ${payload.length}`);
console.log("");
console.log(payload);