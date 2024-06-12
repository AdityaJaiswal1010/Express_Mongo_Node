const express = require("express");
const app = express();
 
 const port= 5000
 var users=[{
    name: 'John',
    kidneys: [{
        healthy:false
    },
    {
        healthy: true
    }
    ]
 }]
 app.use(express.json());
 app.get('/',function(req,res){
    const user1=users[0].kidneys;
    console.log(user1);
    res.send(user1);
 });

 app.post('/',function(req,res){
    const isHealthy=req.body.isHealthy; //getting postman body data 
    users[0].kidneys.push({
        healthy: isHealthy,     // push postman fetched data to local variable users in its memory data
    })
    res.json({
        users: users[0],  //print on screen
    })
 });
 app.get('/getData',function(req,res){
    res.send(users[0]);
    res.send(users);
 })

 app.put('/',function(req,res){
    for(let i=0;i<users[0].kidneys.length;i++)
        {
            if(users[0].kidneys[i].healthy==false)
                {
                    users[0].kidneys[i].healthy=true;
                }
        }
        res.json({msg:"update done!"});
        res.send(users);
 });

 app.delete('/',function(req,res){
    
 });

 app.listen(port);