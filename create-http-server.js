const express = require("express");
const app = express();
const bodyParser = require("body-parser"); // this body parser always parses a json data whenever given or fetched and makes in acccessible and interpretable 
const port = 3000;

app.use(bodyParser.json());
app.get('/something', function(req,res){
    res.json({name:"Aditya",age:22});
   console.log(req.body);
    
})
app.post('/somethingPost', function(req,res){
    console.log(req.body);

    res.send("Hello");
    
})
app.get('/returnHTML',function(req,res){
    res.send('<b>Hello World<b>');
})
app.get('/', function(req,res){
    let a=0;
    // some delay before page render 
    for(let i=0;i<10000;i++)
    {
        a++;
        console.log(a);
    }
    console.log("Out of loop");
    res.send("Hello World! Hey there");
})
function calculate(n){
    let ans=0;
    for(let i=1;i<=n;i++)
    {
        ans+=i;
    }
    return ans;
}
app.get('/calculate', function(req,res){
    const n=req.query.n;
    const ans=calculate(n);
    res.send('<b>Sum of first n number - <b/>'+ans.toString());
})
app.listen(port);



