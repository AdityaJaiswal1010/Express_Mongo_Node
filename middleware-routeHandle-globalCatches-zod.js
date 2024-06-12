const express = require("express");
const bodyParser = require("body-parser"); // Import body-parser
const zod = require("zod"); //zod validator initalize
const app = express();
const port = 5005;

// Schema for validating the request body
const schema = zod.array(zod.number());

// Middleware to check credentials
function credential(req, res, next) {
    if (req.headers.username === "Aditya" && req.headers.password === "pass") { //fetch data from postman 
        console.log('Correct Credentials');
        next(); //routeHandled and told to move to next function or middleware specified
    } 
}

// Middleware to validate input based on schema
function input(req, res, next) {
    const parsedBody = schema.safeParse(req.body); // Parse request body based on schema using zod
    if (!parsedBody.error) {
        console.log("Correct Input");
        next();
    } 
}

// Middleware to parse JSON bodies
app.use(bodyParser.json());

//First pass via middleware check functions like credentials and input then the main function validator
app.get('/check', credential, input, function (req, res) {
    res.send("Everything seems correct");
});

// Error handler middleware or global catch for any erro
app.use(function (err, req, res, next) {
    res.send(schema.safeParse(req.body));
    res.send('Oops! Something went wrong');
});

// Start the server
app.listen(port);
