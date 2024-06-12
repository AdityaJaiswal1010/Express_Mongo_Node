const mongoose=require("mongoose");
const express = require("express");
const bodyParser = require("body-parser"); // Import body-parser
const zod = require("zod"); //zod validator initalize
const app = express();
const port = 5000;
//making a zod validqator schema object
const Validator= zod.object({
    username: zod.string(),
    email: zod.string().email(),
    course: zod.array(zod.string()),
    password: zod.string().min(6)
});

mongoose.connect("mongodb+srv://adityajaiswal9820:Aditya%40101010@cluster0.zt8jocz.mongodb.net/Connection");

//Making a schema for future reference and structurization of mongo
const UserSchema= new mongoose.Schema({
    email: String,
    password: String,
    purchasedCourse: [{
        type: mongoose.Schema.Types.ObjectId,
        ref: 'Course',
    }]
})
const CourseScheme= new mongoose.Schema({
    title: String,
    price: Number,
})

const User= mongoose.model('User',UserSchema);
const Course= mongoose.model('Courses',CourseScheme);

// Middleware to parse JSON bodies
app.use(bodyParser.json());
function credential(req,res,next){
    const parsedBody= Validator.safeParse(req.body);
    if(!parsedBody.error){
        res.send("Acceptable Credentials");
        next();
    }
    else{
        res.send("Oops! Input error");
        return;
    }
}
app.post('/addToDB', credential, async function (req, res) {
    // res.send("Everything seems correct");
    let courseId = [];
        // finding the courses that user wants to add from DB
        for (let i = 0; i < req.body.course.length; i++) {
            let courseDoc = await Course.findOne({ title: req.body.course[i] });
            if (!courseDoc) {
                courseDoc = await Course.create({ title: req.body.course[i], price: 5999 });
            }
            courseId.push(courseDoc._id);
        }
        
        // res.send(req.body.course.length+"Course Created if not exists or Fetched");
    User.create({
        username: req.body.username,
        password: req.body.password,
        purchasedCourse: courseId,
        email: req.body.email,
    })
    // res.send("User added");

});
// get all courses
app.get('/getCourses', async function(req,res){
    const allCourses=await Course.find({});
    res.send(JSON.stringify(allCourses));

});
//get all courses of a particular user 
app.get('/getCoursesOfUser', async function(req, res) {
    try {
        const user = await User.findOne({ email: req.body.email });
        if (!user) {
            return res.status(404).send({ error: 'User not found' });
        }
        const allCoursesObjectID = user.purchasedCourse;
        const userCourses = [];
        for (let i = 0; i < allCoursesObjectID.length; i++) {
            const course = await Course.findOne({ _id: allCoursesObjectID[i] });
            if (course) {
                userCourses.push(course.title);
            }
        }
        res.send(userCourses);
    } catch (error) {
        res.status(500).send({ error: 'An error occurred while fetching user courses' });
    }
});


// Error handler middleware or global catch for any erro
app.use(function (err, req, res, next) {
    // res.send(schema.safeParse(req.body));
    res.send('Oops! Something went wrong');
});

// Start the server
app.listen(port);
