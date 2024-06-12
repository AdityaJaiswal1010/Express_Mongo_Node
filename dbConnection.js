const mongoose=require("mongoose");


mongoose.connect("mongodb+srv://adityajaiswal9820:Aditya%40101010@cluster0.zt8jocz.mongodb.net/Connection");
const User=mongoose.model('users',{name:String,email:String,password:String});

const user=new User({
    name: 'Aditya Shyam Jaiswal',
    email:"adityajaiswal9820@gmail.com",
    password:"123456789",
});

user.save();