#include <iostream>
#include <new>
#include <string>
#include <fstream>
using namespace std;

class userData{
    string name, status;
    int courseCount;
    double gpa;

    public:
        void setName(string line);
        void setStatus(string mail);
        void setGpa();
        void changeCourseCount(int selection);
        string getName();
        string getStatus();
        int getCourseCount();
        double getGpa();
};

class courseData{
    string courseName, courseCode, professorName;
    int studentCount, assistantCount, credit;
    string** assistants;
    string** studentGrades;

    public:
    courseData();
    void setCourseName(string line);
    int setCourseCode(string courseCode);
    void setStudentCount(int studentCount);
    void setAssistantCount(string line);
    void setAssistantNames(string line);
    void setStudentGrades();
    void setCredit(string line);
    void setProfessorName(string line);
    string getCourseName();
    string getCourseCode();
    int getStudentCount();
    int getAssistantCount();
    string** getAssistantNames();
    string** getStudentGrades();
    int getCredit();
    string getProfessorName();
    void clearCourse();
};

void teachingAssistant(userData* user, courseData* courseInfo); //a function for professors to add, replace, and remove a teaching assistant in their course.

string fullUpperCaseName(string name); //a function to make uppercase for the whole name

string upperCaseName(string name); //a function to make the first letter of 1 word of names uppercase.

void mainInterface(userData* user, courseData* courseInfo); //first menu to login or signup

string getName(); //a function for users to write their name without an error during signup

int nameCheck(string name); //a function to check the written name during signup to detect users who already have an account.

int courseCheck(string name);//a function to check the written name during course creation to detect existing courses.

void signUp(); //a function for signing up as a new user.

string logIn(userData* user); //a function for loging in into the system as an existing user.

void stuInterface(userData* user, courseData* courseInfo); //main menu and general interface for students

void taInterface(userData* user, courseData* courseInfo); //main menu and general interface for teaching assistants.

void profInterface(userData* user, courseData* courseInfo); //main menu and general interface for professors.

void createCourse(userData* user); //a function for professors to create a new course.

void enrollCourse(userData* user, courseData* courseInfo); //a function to enroll into a course.

int seeCourses(userData* user, int dropCourse); //a function to see either all the available courses or the ones that you already have access.

void seeGrade(userData* user, courseData* courseInfo); //a function for students to see their grades or assistants an professors to see all the grades in a course.

void changeGrade(userData* user, courseData* courseInfo); //a function for assistants and professors to change grades of students.

void dropCourse(userData* user, courseData* courseInfo); //a function for students to drop a course.

string encode(string password); //a function the encode the password




int main(){
    userData user;
    userData* userPtr = &user;
    courseData course;
    courseData* coursePtr = &course;
    mainInterface(userPtr,coursePtr);
    return 0;
}

void userData::setName(string name){
    this->name = name;
}

void userData::setStatus(string mail){
    size_t pos = mail.find("@") + 1;
    string line, selection, nameCheck, status = mail.substr(pos);
    this->courseCount = 0;
    int isTa = 0;
    if(status == "stu.edu.tr"){
        nameCheck = "<taname:" + this->name + ">";
        ifstream courses("course_info.txt");
        if(courses.is_open()){
            while(getline(courses,line)){
                if(line.find(nameCheck) != string::npos){
                    isTa = 1;
                    this->courseCount++;
                }
            }
        }
        courses.close();
        if(isTa == 1){
            cout << endl << "1) Student" << endl << "2) Teaching Assistant" << endl << "Which one do you want to log in as : ";
            getline(cin, selection);
            while(selection != "1" && selection != "2"){
                cout << endl << "Incorrect input please type either 1 or 2 : ";
                getline(cin, selection);
            }
            if(selection == "1"){
                isTa = 0;
            }
            if(selection == "2"){
                this->status = "Teaching Assistant";
            }
        }
        if(isTa == 0){
            this->courseCount = 0;
            this->status = "Student";
            this->setGpa();
            ifstream courses("course_info.txt");
            nameCheck = "<studentname:" + this->name + ">";
            if(courses.is_open()){
                while(getline(courses,line)){
                    if(line.find(nameCheck) != string::npos)
                        this->courseCount++;
                }
                courses.close();
            }
        }
    }
    if(status == "prof.edu.tr"){
        this->status = "Professor";
        nameCheck = "<professorname:" + this->name + ">";
        ifstream courses("course_info.txt");
        if(courses.is_open()){
            while(getline(courses,line)){
                if(line.find(nameCheck) != string::npos){
                    this->courseCount++;
                }
            }
            courses.close();
        }

    }
}

void userData::setGpa(){
    string line, credit1, studentCheck, courseGrade;
    studentCheck = "<studentname:" + this->name + ">";
    ifstream course("course_info.txt");
    int inTheCourse, credit, totalCredit = 0;
    double grade = 0;
    inTheCourse = 0;
    if(course.is_open()){
        while(getline(course,line)){
            size_t pos, pos2;
            if(line.find("<coursecode:") != string::npos){
                pos = line.find("<credit:") + 8;
                pos2 = line.find(">",pos);
                credit1 = line.substr(pos, pos2 - pos);
                inTheCourse = 1;
                for(char c : credit1){
                    credit = (c - '0');
                }
            }
            if(line.find(studentCheck) != string::npos && inTheCourse == 1){
                pos = line.find("<grade:") + 7;
                pos2 = line.find(">", pos);
                if(pos != pos2){
                    courseGrade = line.substr(pos, pos2 - pos);
                    if(courseGrade == "FF")
                        grade += (0 * credit);
                    if(courseGrade == "DD")
                        grade += (1 * credit);
                    if(courseGrade == "DC")
                        grade += (1.5 * credit);
                    if(courseGrade == "CC")
                        grade += (2 * credit);
                    if(courseGrade == "CB")
                        grade += (2.5 * credit);
                    if(courseGrade == "BB")
                        grade += (3 * credit);
                    if(courseGrade == "BA")
                        grade += (3.5 * credit);
                    if(courseGrade == "AA")
                        grade += (4 * credit);
                    totalCredit += credit;
                }
            }
            if(line.find("<endofcourse>") != string::npos)
                inTheCourse = 0;
        }
        course.close();
        if(totalCredit == 0)
            totalCredit = 1;
        this->gpa = grade / totalCredit;
    }
}

void userData::changeCourseCount(int selection){
    if(selection == 0)
        this->courseCount--;
    if(selection == 1)
        this->courseCount++;
}

string userData::getName(){
    return this->name;
}

string userData::getStatus(){
    return this->status;
}

int userData::getCourseCount(){
    return this->courseCount;
}

double userData::getGpa(){
    return this->gpa;
}

courseData::courseData(){
    this->courseCode = "";
    this->courseName = "";
    this->assistants = nullptr;
    this->studentGrades = nullptr;
    this->assistantCount = 0;
    this->studentCount = 0;
    this->credit = 0;
    this->professorName = "";
}

void courseData::setCourseName(string line){
    size_t pos, pos2;
    pos = line.find("<coursename:") + 12;
    pos2 = line.find(">",pos);
    this->courseName = line.substr(pos, pos2 - pos);
}

int courseData::setCourseCode(string courseCode){
    int courseDoesNotExist = 1;
    int inTheCourse, studentCounter = 0;
    inTheCourse = 0;
    string line;
    ifstream courses("course_info.txt");
    if(courses.is_open()){
        while(getline(courses,line)){
            if(line.find("<coursecode:" + courseCode + ">") != string::npos){
                courseDoesNotExist = 0;
                inTheCourse = 1;
                this->courseCode = courseCode;
                this->setCourseName(line);
                this->setAssistantCount(line);
                this->setAssistantNames(line);
                this->setCredit(line);
                this->setProfessorName(line);
            }
            else if(inTheCourse == 1 && line.find("<studentname:") != string::npos){
                studentCounter++;
            }
            else if(inTheCourse == 1 && line.find("<endofcourse") != string::npos){
                inTheCourse = 0;
            }
        }
    }
    setStudentCount(studentCounter);
    courses.close();
    setStudentGrades();
    return courseDoesNotExist;
}

void courseData::setStudentCount(int studentCount){
    this->studentCount = studentCount;
}

void courseData::setAssistantCount(string line){
    size_t pos = 0;
    this->assistantCount = 0;
    while(line.find("<taname:", pos) != string::npos){
        pos = line.find("<taname:", pos) + 8;
        this->assistantCount++;
    }
}

void courseData::setAssistantNames(string line){
    string name;
    size_t pos = 0;
    size_t pos2;
    if(assistantCount != 0){
        this->assistants = new(nothrow) string*[this->assistantCount];
        if(this->assistants != nullptr){
            for(int i = 0; i < assistantCount; i++){
                pos = line.find("<taname:",pos) + 8;
                pos2 = line.find(">",pos);
                name = line.substr(pos, pos2 - pos);
                this->assistants[i] = new(nothrow) string[name.length()];
                if(this->assistants[i] != nullptr){
                    this->assistants[i][0] = name;
                    pos = pos+1;
                }
            }
        }
    }
}

void courseData::setStudentGrades(){
    string line, studentGrade, studentName, gradeDisplay;
    int inTheCourse = 0;
    size_t pos = 0;
    size_t pos2;
    if(studentCount != 0){
        ifstream courses("course_info.txt");
        this->studentGrades = new(nothrow) string*[studentCount];
        if(courses.is_open() && this->studentGrades != nullptr){
            int i = 0;
            while(getline(courses,line)){
                if(line.find("<coursecode:" + this->courseCode + ">") != string::npos){
                    inTheCourse = 1;
                }
                else if(inTheCourse == 1 && line.find("<studentname:") != string::npos){
                    pos = line.find("<studentname:") + 13;
                    pos2 = line.find(">",pos);
                    studentName = line.substr(pos,pos2 - pos);
                    pos = line.find("<grade:") + 7;
                    pos2 = line.find(">",pos);
                    if(pos == pos2){
                        studentGrade = "Not Graded";
                    }
                    else{
                        studentGrade = line.substr(pos, pos2 - pos);
                    }

                    gradeDisplay = studentName + " : " + studentGrade;
                    studentGrades[i] = new(nothrow) string[gradeDisplay.length()];
                    if(studentGrades[i] != nullptr)
                        studentGrades[i][0] = gradeDisplay;
                }
                else if(inTheCourse == 1 && line.find("<endofcourse>") != string::npos){
                    inTheCourse = 0;
                }
            }
            courses.close();
        }
    }
}

void courseData::setCredit(string line){
    size_t pos, pos2;
    string credit;
    pos = line.find("<credit:") + 8;
    pos2 = line.find(">",pos);
    credit = line.substr(pos, pos2 - pos);
    for(char c : credit){
        this->credit = c - '0';
    }
}

void courseData::setProfessorName(string line){
    size_t pos, pos2;
    pos = line.find("<professorname:") + 15;
    pos2 = line.find(">",pos);
    this->professorName = line.substr(pos, pos2 - pos);
}

string courseData::getCourseName(){
    return this->courseName;
}

string courseData::getCourseCode(){
    return this->courseCode;
}

int courseData::getStudentCount(){
    return this->studentCount;
}

int courseData::getAssistantCount(){
    return this->assistantCount;
}

string** courseData::getAssistantNames(){
    return this->assistants;
}

string** courseData::getStudentGrades(){
    return this->studentGrades;
}

int courseData::getCredit(){
    return this->credit;
}

string courseData::getProfessorName(){
    return this->professorName;
}

void courseData::clearCourse(){
    this->courseCode = "";
    this->courseName = "";
    if(assistantCount != 0){
        for(int i = 0; i < this->assistantCount; i++){
            delete[] this->assistants[i];
        }
        delete[] assistants;
    }
    this->assistants = nullptr;
    if(studentCount != 0){
        for(int i = 0; i < this->studentCount; i++){
            delete[] this->studentGrades[i];
        }
        delete[] this->studentGrades;
    }
    this->studentGrades = nullptr;
    this->assistantCount = 0;
    this->studentCount = 0;
    this->credit = 0;
    this->professorName = "";
}

void teachingAssistant(userData* user, courseData* courseInfo){
    string courseCode, courseCodeCheck, courseName, selection, line, line2;
    int x = 0;
    int y = 0;
    size_t pos,pos2;

    cout << endl << "*************************************************************";
    seeCourses(user,0);

    string** assistants;

    do{
        if(x == 0)
            cout << endl << "Type the course code of the course you want to work on : ";
        if(x == 1)
            cout << endl << "Incorrect input! Please type the course code of the course you want to work on correctly : ";
        getline(cin,courseCode);
        courseCode = upperCaseName(courseCode);
        x = (*courseInfo).setCourseCode(courseCode);
        assistants = (*courseInfo).getAssistantNames();
        if(x != 1 && (*courseInfo).getProfessorName() != (*user).getName()){
            x = 1;
        }
        if(x != 1 && (*courseInfo).getProfessorName() == (*user).getName()){
            if((*courseInfo).getAssistantCount() != 0 && x == 0){
                cout << endl << "Names of the assistants you have on " << (*courseInfo).getCourseName() << " :" ;
                for(int i = 0; i < (*courseInfo).getAssistantCount(); i++){
                    cout << endl << i + 1 << ") " << assistants[i][0];
                }
            }
            else if(x == 0){
                cout << endl << "You don't have any assistants on " << courseName;
            }
            cout << endl << "------------------------------------------" << endl;
        }
        if(x == 1)
            (*courseInfo).clearCourse();
    }while(x == 1);
    string assistantName, assistantRemove, assistantReplace;
    if((*courseInfo).getAssistantCount() == 0){
        cout << endl << "1) Add a Teaching Assistant" << endl << "2) Replace a Teaching Assistant" << endl << "Choose what to do : ";
        getline(cin,selection);
        while(selection != "1" && selection != "2"){
            cout << endl << "Incorrect input! Please type 1 or 2 : ";
            getline(cin,selection);
        }
    }
    else{
        cout << endl << "1) Add a Teaching Assistant" << endl << "2) Replace a Teaching Assistant" << endl << "3) Remove a Teaching Assistant" << endl << "Choose what to do : ";
        getline(cin,selection);
        while(selection != "1" && selection != "2" && selection != "3"){
            cout << endl << "Incorrect input! Please type a number between 1 and 3 : ";
            getline(cin,selection);
        }
    }
    int assistantRemoveDecide = 0;
    while(selection == "1" || selection == "2"){
        if(selection == "2" && assistantRemoveDecide == 0){
            cout << endl << "Type the name of the Teaching Assistant that you want to change : ";
            getline(cin,assistantRemove);
            assistantReplace = fullUpperCaseName(assistantRemove);
            assistantRemove = "<taname:" + fullUpperCaseName(assistantRemove) + ">";
            int assistantDontExist = 1;
            while(assistantDontExist == 1){
                for(int i = 0; i < (*courseInfo).getAssistantCount(); i++){
                    if(assistants[i][0] == assistantReplace){
                        assistantDontExist = 0;
                    }
                }
                if(assistantDontExist == 1){
                    string selection2 = selection;
                    cout << endl << "There are no assistants in this course whose name is " << assistantReplace << "! Press 1 to type another name. Press 2 to skip replacing a teaching assistant : ";
                    getline(cin,selection2);
                    while(selection2 != "1" && selection2 != "2"){
                        cout << endl << "Incorrect input! Please type either 1 or 2 : ";
                        getline(cin,selection2);
                    }
                    if(selection2 == "1"){
                        cout << endl << "Type the name of the Teaching Assistant that you want to change : ";
                        getline(cin,assistantRemove);
                        assistantReplace = fullUpperCaseName(assistantRemove);
                        assistantRemove = "<taname:" + fullUpperCaseName(assistantRemove) + ">";
                    }
                    if(selection2 == "2"){
                        selection = 4;
                    }
                }
            }
            assistantRemoveDecide = 1;
        }
        if(selection == "1" && (*courseInfo).getAssistantCount() >= 4){
            cout << endl << "You have reached the maximum amount of assistants on this course." << endl << "1) Replace a Teaching Assistant" << endl << "2) Remove a Teaching Assistant" << endl << "3) Go back to previous menu" << endl << "Choose what to do : ";
            getline(cin,selection);
            while(selection != "1" && selection != "2" && selection != "3"){
                cout << endl << "Incorrect input! Please type a number between 1 and 3 : ";
                getline(cin,selection);
            }
            if(selection == "1"){
                selection = "2";
            }
            else if(selection == "2"){
                selection = "3";
            }
            else if(selection == "3"){
                selection = "4";
            }
        }
        if(selection != "4"){
            cout << endl << "Type the name of the student you want as a teaching assistant in your course : ";
            getline(cin,assistantName);
            assistantName = fullUpperCaseName(assistantName);
            string selection2 = selection;
            while(nameCheck(assistantName) == 1 && (selection == "1" || selection == "2")){
                if(selection == "1")
                    cout << endl << "There are no student in the school whose name is " << assistantName << endl << "Press 1 to type another name. Press 2 to skip adding a teaching assistant : ";
                if(selection == "2")
                    cout << endl << "There are no student in the school whose name is " << assistantName << endl << "Press 1 to type another name. Press 2 to skip replacing a teaching assistant : ";
                getline(cin,selection2);
                while(selection2 != "1" && selection2 != "2"){
                    cout << endl << "Incorrect input! please type 1 or 2 : ";
                    getline(cin,selection2);
                }
                if(selection2 == "1"){
                    cout << endl << "Type another student name : ";
                    getline(cin,assistantName);
                    assistantName = fullUpperCaseName(assistantName);
                }
                if(selection2 == "2")
                    selection = "4";
            }

            
            int ifAssistant = 0;
            int inTheCourse = 0;
            int ifStudent = 0;
            for(int i = 0; i < (*courseInfo).getStudentCount(); i++){
                if((*courseInfo).getStudentGrades()[i][0].find(assistantName) != string::npos){
                    cout << "You cannot make a student, who is currently taking your class, a teaching assistant. You need to choose someone else.";
                    ifStudent = 1;
                }
            }
            courseCodeCheck = "<coursecode:" + (*courseInfo).getCourseCode() + ">";
            string assistantCheck = "<taname:" + assistantName + ">";
            for(int i = 0; i < (*courseInfo).getAssistantCount(); i++){
                if((*courseInfo).getAssistantNames()[i][0].find(assistantName) != string::npos){
                    cout << endl << "You already have " << assistantName << " as an assistant in " << (*courseInfo).getCourseCode();
                    cout << endl << "1) Choose another student" << endl << "2) Replace a Teaching Assistant" << endl << "3) Remove a Teaching Assistant" << endl << "Choose what to do : ";
                    getline(cin,selection);
                    while(selection != "1" && selection != "2" && selection != "3"){
                        cout << endl << "Incorrect input! Please choose a number between 1 and 3 : ";
                        getline(cin,selection);
                    }
                        ifAssistant = 1;
                }
            }
            ifstream courses("course_info.txt");
            if(courses.is_open()){
                if(ifAssistant == 0 && ifStudent == 0 && selection == "1"){
                    ofstream temp("temp.txt");
                    if(courses.is_open() && temp.is_open()){
                        while(getline(courses,line)){
                            if(line.find(courseCodeCheck) != string::npos){
                                line += assistantCheck;
                            }
                            temp << line << endl;
                        }
                        courses.close();
                        temp.close();
                        remove("course_info.txt");
                        rename("temp.txt","course_info.txt");
                        selection = "4";
                        cout << endl << assistantName << " successfully added as a Teaching Assistant to the " << courseName << "course.";
                    }
                }
                if(ifAssistant == 0 && ifStudent == 0 && selection == "2"){
                    ofstream temp("temp.txt");
                    if(courses.is_open() && temp.is_open()){
                        while(getline(courses,line)){
                            if(line.find(courseCodeCheck) != string::npos){
                                line.replace(line.find(assistantRemove), assistantRemove.length(), assistantCheck);
                            }
                            temp << line << endl;
                        }
                        courses.close();
                        temp.close();
                        remove("course_info.txt");
                        rename("temp.txt","course_info.txt");
                        selection = "4";
                        cout << endl << assistantName << " successfully replaced with " << assistantReplace << " as a Teaching Assistant to the " << courseName << "course.";
                    }
                }
            }
        }
    }
    while(selection == "3"){       
        cout << endl << "Type the name of the Teaching Assistant that you want to remove : ";
        getline(cin,assistantName);
        assistantName = fullUpperCaseName(assistantName);
        assistantRemove = "<taname:" + assistantName + ">";
        int ifAssistant = 0;
        while(ifAssistant == 0){
            for(int i = 0; i < (*courseInfo).getAssistantCount(); i++){
                string assistantName2;
                size_t pos = (*courseInfo).getAssistantNames()[i][0].find(" : ");
                assistantName2 = (*courseInfo).getAssistantNames()[i][0].substr(0, pos);
                if(assistantName == assistantName2){
                    ifAssistant = 1;
                }
            }
            if(ifAssistant == 0){
                cout << endl << "There are no assistants in this course whose name is " << assistantName << "! Press 1 to type another name. Press 2 to skip removing a teaching assistant : ";
                getline(cin,selection);
                    while(selection != "1" && selection != "2"){
                        cout << endl << "Incorrect input! Please type either 1 or 2 : ";
                        getline(cin,selection);
                    }
                    if(selection == "1"){
                        selection = "3";
                        cout << endl << "Type the name of the Teaching Assistant that you want to change : ";
                        getline(cin,assistantName);
                        assistantName = fullUpperCaseName(assistantName);
                        assistantRemove = "<taname:" + assistantName + ">";
                    }
                    if(selection == "2"){
                        selection = "4";
                        ifAssistant = 1;
                    }
            }
        }
        if(selection != "4"){
            ifstream courses("course_info.txt");
            ofstream temp("temp.txt");
            if(courses.is_open() && temp.is_open()){
                while(getline(courses,line)){
                    if(line.find("<coursecode:" + (*courseInfo).getCourseCode() + ">") != string::npos){
                        size_t pos = line.find(assistantRemove);
                        line.erase(pos, assistantRemove.length());
                        selection = "4";
                    }
                    temp << line << endl;
                }
                courses.close();
                temp.close();
                remove("course_info.txt");
                rename("temp.txt","course_info.txt");
                cout << endl << assistantName << " successfully removed from being Teaching Assistant in the " << courseName << "course.";
            }
        }
    }
    (*courseInfo).clearCourse();
    cout << endl << "*************************************************************" << endl << endl;
}

string fullUpperCaseName(string name){
    size_t pos, pos2;
    pos = pos2 = 0;
    string name2;
    while(name.find(" ", pos) != string::npos){
        pos2 = name.find(" ",pos);
        name2 = name.substr(pos, pos2 - pos);
        name.replace(pos, pos2 - pos, upperCaseName(name2));
        pos = pos2 + 1;
    }
    name2 = name.substr(pos);
    name.replace(pos, name2.length() , upperCaseName(name2));
    
    return name;
}

string upperCaseName(string name){
    string name2 = "";
    char cc;
    int j = 0;
    for(char c : name){
        if(c <= 'z' && c >= 'a' && j == 0){
            cc = (c - ('z' - 'Z'));
            name2  = name2 + cc;

        }
        else if(c <= 'Z' && c >= 'A' && j != 0){
            cc = (c - ('Z' - 'z'));
            name2  = name2 + cc;
        }
        else{
            name2 = name2 + c;
        }
        j++;
    }
    return name2;
}

void mainInterface(userData* user, courseData* courseInfo){
    string selection, name;
    int selection1;
    cout << "*************************************************************" << endl;
    cout << "Welcome to our university." << endl << endl;
    cout << "1) Signup" << endl << "2) Login" << endl;
    cout << "Choose what to do : ";
    getline(cin,selection);
    while(selection != "1" && selection != "2"){
        cout <<"Incorrect input please choose either 1 or 2 : ";
        getline(cin,selection);
        cout << endl;
    }
    cout << "*************************************************************" << endl << endl;
    if(selection == "1"){
        signUp();
        selection = "2";
    }
    if(selection == "2"){
        name = logIn(user);
        while(name == "signup"){
            signUp();
            name = logIn(user);
        }
        if((*user).getStatus() == "Student"){
            stuInterface(user, courseInfo);
        }
        if((*user).getStatus() == "Teaching Assistant"){
            taInterface(user, courseInfo);
        }
        if((*user).getStatus() == "Professor"){
            profInterface(user, courseInfo);
        }
    }
}

int nameCheck(string name){
    string line, name2, fileName = "login_data.txt";
    ifstream txtFile(fileName);
    size_t pos,pos2;
    if(txtFile.is_open()){
        while(getline(txtFile,line)){
            pos = line.find("<name:") + 6;
            pos2 = line.find(">", pos);
            name2 = line.substr(pos, pos2 - pos);
            if(name == name2){
                return 0;
            }
        }
    }
    txtFile.close();
    return 1;
}

int courseCheck(string name, int selection){
    string line, name2, fileName = "course_info.txt";
    ifstream txtFile(fileName);
    size_t pos,pos2;
    if(txtFile.is_open()){
        while(getline(txtFile,line)){
            if(selection == 1){
                pos = line.find("<coursecode:") + 12;
                pos2 = line.find(">", pos);
                if(pos != string::npos && pos2 != string::npos)
                    name2 = line.substr(pos, pos2 - pos);
            }
            if(selection == 2){
                pos = line.find("<coursename:") + 12;
                pos2 = line.find(">", pos);
                if(pos != string::npos && pos2 != string::npos)
                    name2 = line.substr(pos, pos2 - pos);
            }
            if(name == name2){
                return 1;
            }
        }
    }

    return 0;
}

void signUp(){
    string mail,password,name,selection,status;
    size_t pos;
    int selection1 = 0;
    cout << "*************************************************************" << endl;
    cout << "1) Student" << endl << "2) Professor" << endl;
    cout << "What's your status : ";
    getline(cin,selection);
    while(selection != "1" && selection != "2"){
        cout <<"Incorrect input please choose either 1 or 2: ";
        getline(cin,selection);
        cout << endl;
    }
    if(selection == "1"){
        status = "@stu.edu.tr";
    }
    if(selection == "2"){
        status = "@prof.edu.tr";
    }

    cout << endl << "Type your full name and surname : ";
    getline(cin, name);
    name = fullUpperCaseName(name);
    if(nameCheck(name)){
        cout << endl << "Type your username without using '@' and space characters to signup : ";
        getline(cin, mail);
        while(mail.find(" ") != string::npos || mail.find("@") != string::npos){
            cout << endl << "Type your username again without using '@' and space characters to signup : ";
            getline(cin, mail);
        }
        mail = mail + status;
        cout << endl << "Type your password to signup : ";
        getline(cin, password);
        cout << endl;
        string encodedPassword = encode(password);

        while(selection1 != 4){
            cout << "-------------------------------------------------------------" << endl;
            cout << "Your chosen e-mail is : " << mail << endl << "Your chosen password is : " << password << endl << "Your chosen name is : " << name << endl;
            cout << "-------------------------------------------------------------" << endl << endl;
            cout << endl << "1) e-mail" << endl << "2) password" << endl << "3) name" << endl;
            cout << "Choose one of the options above to change, type 4 to confirm your mail, password, and name which are written above : ";
            getline(cin,selection);
            while(selection != "1" && selection != "2" && selection != "3" && selection != "4"){
                cout <<"Incorrect input please choose a number between 1 and 4: ";
                getline(cin,selection);
                cout << endl;
            }
            if(selection == "1"){
                selection1 = 1;
            }
            if(selection == "2"){
                selection1 = 2;
            }
            if(selection == "3"){
                selection1 = 3;
            }
            if(selection == "4"){
                selection1 = 4;
            }
            switch(selection1){
                case 1: 
                    cout << "Type your username without using '@' and space characters to signup : ";
                    getline(cin, mail);
                    while(mail.find(" ") != string::npos || mail.find("@") != string::npos){
                        cout << endl << "Type your username without using '@' and space characters to signup : ";
                        getline(cin, mail);
                    }
                    mail = mail + status;
                    
                    break;
                case 2: 
                    cout << endl << "Type your password to signup : ";
                    getline(cin, password);
                    cout << endl;
                    encodedPassword = encode(password);

                    break;
                case 3:
                    cout << endl << "Type your full name and surname : ";
                    getline(cin, name);
                    name = fullUpperCaseName(name);
                    break;
                case 4:
                    cout << "Successfully signed up. Continue by logging in.";
                    
                    break;
                default:

                    break;
            }
        }
        string fileName = "login_data.txt";
        ofstream txtFile(fileName, ios::app);
        txtFile << "<mail:" << mail << "><password:" << encodedPassword << "><name:" << name << ">" << endl;
        txtFile.close();
    }
    else{
        cout << endl << "You already have an account, login instead of signup";
    }
    cout << endl << "*************************************************************" << endl << endl;
} 

string logIn(userData* user){
    string mail, mail2, password, password2;
    string fileName = "login_data.txt";
    string name;
    size_t pos,pos2;
    int loginMail;
    cout << endl << "*************************************************************" << endl;
    cout << "Type your e-mail to login : ";
    getline(cin,mail);
    cout << endl;
    string selection;
    do{
        ifstream txtFile(fileName);
        if(txtFile.is_open()){
            string line;

            while(getline(txtFile, line)){
                pos = line.find("<mail:");
                pos2 = line.find(">");
                mail2 = line.substr(pos + 6, pos2 - (pos + 6));
                if(mail == mail2){
                    pos = line.find("<password:");
                    pos2 = line.find(">", pos + 1);
                    password2 = line.substr(pos + 10, pos2 - (pos + 10));
                    cout << "Type your password : ";
                    getline(cin,password);
                    cout << endl;
                    string encodedPassword = encode(password);
                    while(encodedPassword != password2){
                        cout << "Incorrect password type your password again : ";
                        getline(cin,password);
                        encodedPassword = encode(password);
                    }
                    if(encodedPassword == password2){
                        pos = line.find("<name:");
                        pos2 = line.find(">", pos + 1);
                        pos = pos + 6;
                        name = line.substr(pos, pos2 - pos);
                        txtFile.close();
                        (*user).setName(name);
                        (*user).setStatus(mail);
                        txtFile.close();
                        cout << "*************************************************************" << endl << endl;
                        return name;
                    }
                }
            }
            cout << "Incorrect e-mail" << endl;
            cout << "1) Type your e-mail again" << endl;
            cout << "2) Signup if you don't have an account" << endl;
            cout << "Select what to do : ";
            getline(cin,selection);
            while(selection != "1" && selection != "2"){
                cout << endl;
                cout << "Incorrect selection, choose either 1 or 2: ";
                getline(cin,selection);
            }
            if(selection == "1"){
                cout << endl << "Type your e-mail again : ";
                getline(cin,mail);
            }
        }
    }while(selection == "1");

    return "signup";
}


void stuInterface(userData* user, courseData* courseInfo){
    string selection;
    int selection1;
    do{
        cout << "*************************************************************" << endl;
        cout << "Welcome " << (*user).getStatus() << " " << (*user).getName() << endl;
        if((*user).getCourseCount() != 0){
            cout << endl << "1) See your grades" << endl;
            cout << "2) Enroll to a new course" << endl;
            cout << "3) Drop a course" << endl;
            cout << "4) Log out" << endl;
        }
        if((*user).getCourseCount() == 0){
            cout << endl << "1) Enroll to a new course" << endl;
            cout << "2) Log out" << endl;
        }
        cout << "Select what to do : ";
        getline(cin,selection);
        if((*user).getCourseCount() != 0){
            while(selection != "1" && selection != "2" && selection != "3" && selection != "4"){
                cout << endl << "Incorrect input please choose a number between 1 and 4 : ";
                getline(cin,selection);
            }
        }
        if((*user).getCourseCount() == 0){
            while(selection != "1" && selection != "2"){
                cout << endl << "Incorrect input please choose either 1 or 2 : ";
                getline(cin,selection);
            }
            if(selection == "1")
                selection = "2";
            else if(selection == "2")
                selection = "4";
        }
        for(char c : selection){
            selection1 = c - '0';
        }
        cout << "*************************************************************" << endl << endl;
        switch (selection1){
        case 1:
            seeGrade(user, courseInfo);

            break;
        case 2:
            enrollCourse(user, courseInfo);
            break;
        case 3:
            dropCourse(user, courseInfo);
            (*user).changeCourseCount(0);
            break;
        case 4:
            cout << endl << "Successfully logged out.";
            break;
        default:
            break;
        }
    }while(selection1 < 4);
}; 

void taInterface(userData* user, courseData* courseInfo){
    string selection;
    int selection1;
    do{
        cout << "*************************************************************" << endl;
        cout << "Welcome " << (*user).getStatus() << " " << (*user).getName() << endl;
        cout << endl << "1) Change grades of students" << endl;
        cout << "2) Log out" << endl;
        cout << "Select what to do : ";
        getline(cin,selection);
        while(selection != "1" && selection != "2"){
            cout << endl << "Incorrect input please choose either 1 or 2 : ";
            getline(cin,selection);
        }
        for(char c : selection){
            selection1 = c - '0';
        }
        cout << "*************************************************************" << endl << endl;
        switch (selection1){
        case 1:
            changeGrade(user, courseInfo);
            
            break;
        case 2:
            cout << endl << "Successfully logged out.";
            break;
        default:
            break;
        }
    }while(selection1 < 2);
}

void profInterface(userData* user, courseData* courseInfo){
    string selection, empty;
    int selection1;
    do{
        cout << endl << "*************************************************************" << endl;
        cout << "Welcome " << (*user).getStatus() << " " << (*user).getName() << endl;
        cout << endl << "1) Create a new course" << endl;
        if((*user).getCourseCount() != 0){
            cout << "2) Add, change, or remove a teaching assistant for your courses" << endl;
            cout << "3) Change grades in your courses" << endl;
            cout << "4) Log out" << endl;
        }
        if((*user).getCourseCount() == 0)
            cout << "2) Log out" << endl;
        cout << "Select what to do : ";
        getline(cin,selection);
        if((*user).getCourseCount() != 0){
            while(selection != "1" && selection != "2" && selection != "3" && selection != "4"){
                cout << endl << "Incorrect input please choose a number between 1 and 4 : ";
                getline(cin,selection);
            }
        }
        if((*user).getCourseCount() == 0){
            while(selection != "1" && selection != "2"){
                cout << endl << "Incorrect input please choose either 1 or 2 : ";
                getline(cin,selection);
            }
            if(selection == "2")
                selection = "4";
        }
        for(char c : selection){
            selection1 = c - '0';
        }
        cout << "*************************************************************" << endl << endl;
        switch (selection1){
        case 1:
            createCourse(user);
            (*user).changeCourseCount(1);
            break;
        case 2:
            teachingAssistant(user, courseInfo);

            break;
        case 3:
            changeGrade(user, courseInfo);

            break;
        case 4:
            cout << endl << "Successfully logged out.";
            break;
        default:
            break;
        }
    }while(selection1 < 4);
}

void createCourse(userData* user){
    string courseCode, line, courseName, selection , credit, name, ta, taNumber, fileName = "course_info.txt";
    string fileName2 = "login_data.txt";
    int credit1, error = 1;
    size_t pos = 0;
    size_t pos2 = 0;
    ofstream txtFile(fileName, ios::app);
    if(txtFile.is_open()){
        int x = 0;
        cout << "*************************************************************";
        do{
            if(x < 1)
                cout << endl << "What will be the course code type it without any space character and it can't be more than 8 characters : ";
            else
                cout << endl << "A course with the same course code already exists choose another one : ";
            getline(cin,courseCode);
            while(courseCode.length() > 8 || courseCode.find(" ") != string::npos){
                if(courseCode.length() > 8){
                    cout << endl << "Character limit exceeded. Type the course code again : ";
                    getline(cin,courseCode);
                }
                else if(courseCode.find(" ") != string::npos){
                    cout << endl << "Space character used in the course code. Type the course code again : ";
                    getline(cin,courseCode);
                }
            }
            courseCode = upperCaseName(courseCode);
            x++;
        }while(courseCheck(courseCode,1));
        txtFile << "<coursecode:" << courseCode << ">";
        x = 0;
        do{
            if(x < 1)
                cout << endl << "What will be the name of the course : ";
            else
                cout << endl << "A course with that name already exists please choose another name : ";
            getline(cin,courseName);
            courseName = fullUpperCaseName(courseName);
            x++;
        }while(courseCheck(courseName,2));
        txtFile << "<coursename:" << courseName << ">";
        cout << endl << "How many credits will this course have : ";
        while(error == 1){
            getline(cin,credit);
            for(char c : credit){
                if(c <= '9' && c >= '0'){
                    error = 0;
                }
                else{
                    error = 1;
                    cout << endl << "Incorrect input. Please use only numbers : ";
                    break;
                }
            }
        }
        error = 1;
        txtFile << "<credit:" << credit << ">";
        txtFile << "<professorname:" << (*user).getName() << ">";
        cout << endl << "Do you want to select a teaching assistant for the course (you can do it later as well) type yes or no :";
        getline(cin,ta);
        while(ta != "yes" && ta != "no"){
            cout << endl << "Incorrect input. Type only yes or no : ";
            getline(cin,ta);
        }

        if(ta == "yes"){
            cout << endl << "How many teaching assistants do you want to have in your course (it can't be more than 4) : ";
            getline(cin,taNumber);
            int taNumber1 = 0;
            do{
                if(taNumber.length() > 1){
                    cout << endl << "Incorrect input please type a number between 1 and 4 : ";
                    getline(cin,taNumber);
                }
                if(taNumber.length() == 1){
                    for(char c : taNumber){
                        if(c <= '4' && c >= '1'){
                            taNumber1 = c - '0';
                        }
                        else{
                            cout << endl << "Incorrect input please type a number between 1 and 4 : ";
                            getline(cin,taNumber);
                        }
                    }
                }
            }while(taNumber1 == 0);
            cout << endl << "You will have " << taNumber1 << " teaching assistants in " << courseName;
            string taCheck1, taCheck2, taCheck3;
            for(int i = 0; i < taNumber1; i++){
                error = 1;
                while(error == 1){
                    cout << endl << "Type the name of the teaching assistant : ";
                    getline(cin, name);
                    name = fullUpperCaseName(name);
                    if(i == 0){
                        taCheck1 = name;
                    }
                    if(i == 1){
                        while(name == taCheck1){
                            cout << endl << "You already chose " << name << " as a teaching assistant for your course. Choose someone else : ";
                            getline(cin, name);
                            name = fullUpperCaseName(name);
                            taCheck2 = name;
                        }
                    }
                    if(i == 2){
                        while(name == taCheck1 || name == taCheck2){
                            cout << endl << "You already chose " << name << " as a teaching assistant for your course. Choose someone else : ";
                            getline(cin, name);
                            name = fullUpperCaseName(name);
                            taCheck3 = name;
                        }
                    }
                    if(i == 3){
                        while(name == taCheck1 || name == taCheck2){
                            cout << endl << "You already chose " << name << " as a teaching assistant for your course. Choose someone else : ";
                            getline(cin, name);
                            name = fullUpperCaseName(name);
                        }
                    }
                    ifstream nameData(fileName2);
                    if(nameData.is_open()){
                        while(getline(nameData,line)){
                            pos = line.find("<name:") + 6;
                            pos2 = line.find(">",pos);
                            line = line.substr(pos,pos2 - pos);
                            if(name == line){
                                error = 0;
                                txtFile << "<taname:" << name << ">";
                            }
                        }
                        nameData.close();
                        if(error == 1){
                            cout << endl << "There are no student in the school whose name is " << name << endl << "Press 1 to type another name. Press 2 to skip adding a teaching assistant : ";
                            getline(cin,selection);
                            while(selection != "1" && selection != "2"){
                                cout << endl << "Incorrect input. Please choose 1 or 2 : ";
                                getline(cin,selection);
                            }
                            if(selection == "2"){
                                error = 0;
                            }
                        }
                        nameData.close();
                    }
                }
            }
        }
        txtFile << endl << "<endofcourse>" << endl;
        cout << endl << "Course creation completed.";
        txtFile.close();
        cout << endl << "*************************************************************" << endl << endl;
    }
}


void enrollCourse(userData* user, courseData* courseInfo){
    string courseCode, line, nameCheck, nameCheck2, taCheck;
    int courseCodeCheck;
    cout << endl;
    cout << "*************************************************************";
    int courseCount = seeCourses(user,0);
    cout << endl;
    int x = 0;
    int y = 0;
    if(courseCount == 1){
        cout << endl << "There are no courses that you can enroll. You are going back to the previous menu." << endl << endl;
    }
    else{
        string selection = "1";
        do{
            if(x == 0)
                cout << endl << "Type the course code of the course that you want to enroll : ";
            if(x == 1)
                cout << endl << "There are no courses with that course code.";
            if(x == 2)
                cout << endl << "You are already enrolled to this course.";
            if(x == 3)
                cout << endl << "You can't enroll to this class since you're the teaching assistant.";
            if(x == 1 || x == 2 || x == 3){
                cout << endl << "Type 1 to type another course code. Type 2 to go back to previous menu : ";
                getline(cin, selection);
                while(selection != "1" && selection != "2"){
                    cout << endl << "Incorrect Input! Please type 1 or 2 : ";
                    getline(cin, selection);
                }
                if(selection == "1")
                    cout << endl << "Type the course code of the course that you want to enroll : ";
                if(selection == "2")
                    y = 0;
            }
            if(selection == "1"){
                getline(cin,courseCode);
                courseCode = upperCaseName(courseCode);
                y = (*courseInfo).setCourseCode(courseCode);
                nameCheck = "<coursecode:" + (*courseInfo).getCourseCode() + ">";
                nameCheck2 = "<studentname:" + (*user).getName() + ">";
                x = 1;
                for(int i = 0; i < (*courseInfo).getStudentCount(); i++){
                    if((*courseInfo).getStudentGrades()[i][0].find((*user).getName()) != string::npos){
                        y = 1;
                        x = 2;
                    }
                }
                for(int i = 0; i < (*courseInfo).getAssistantCount(); i++){
                    if((*courseInfo).getAssistantNames()[i][0].find((*user).getName()) != string::npos){
                        y = 1;
                        x = 3;
                    }
                }
            }

        }while(y == 1);
        if(selection == "1"){
            ifstream courses("course_info.txt");
            ofstream temp("temp.txt",ios::app);
            if(courses.is_open()){
                if(temp.is_open()){
                    while(getline(courses,line)){
                        if(line.find(nameCheck) != string::npos){
                            temp << line << endl;
                            courseCodeCheck = 1;
                        }
                        else if(line.find("<endofcourse>") != string::npos && courseCodeCheck == 1){
                            temp << "<studentname:" << (*user).getName() << "><grade:>" << endl;
                            courseCodeCheck = 0;
                            temp << line << endl;
                        }
                        else{
                            temp << line << endl;
                        }
                    }
                    temp.close();
                    courses.close();
                    remove("course_info.txt");
                    rename("temp.txt", "course_info.txt");
                }
            }  
            cout << endl << "Successfully enrolled to the course." << endl;
            (*user).changeCourseCount(1);
            (*courseInfo).clearCourse();
        }
    }
    (*user).setGpa();
    cout << "*************************************************************" << endl << endl;
}

int seeCourses(userData* user, int dropCourse){
    string courseName, courseCode, line;
    size_t pos, pos2;
    int enrollCheck = 0;
    int taCheck = 0;
    int x = 1;
    ifstream course("course_info.txt");
    if(course.is_open()){
        if((*user).getStatus() == "Student"){
            while(getline(course,line)){
                if(line.find("<coursecode:") != string::npos){
                    pos = line.find("<coursecode:") + 12;
                    pos2 = line.find(">",pos);
                    courseCode = line.substr(pos, pos2 - pos);
                    pos = line.find("<coursename:") + 12;
                    pos2 = line.find(">",pos);
                    courseName = line.substr(pos, pos2 - pos);
                    if(line.find("<taname:" + (*user).getName() + ">") != string::npos)
                        taCheck = 1;
                }
                if(line.find("<studentname:" + (*user).getName() + ">") != string::npos)
                    enrollCheck = 1;
                if(enrollCheck == 1 && taCheck == 0 && line.find("<endofcourse>") != string::npos){
                    enrollCheck = 0;
                    if(dropCourse == 1){
                        cout << endl << x << ") (" << courseCode << ") " << courseName;
                        x++;
                    }
                }
                else if(enrollCheck == 0 && taCheck == 0 && line.find("<endofcourse>") != string::npos  && dropCourse == 0){
                    cout << endl << x << ") (" << courseCode << ") " << courseName;
                    x++;
                }
                else if(line.find("<endofcourse>") != string::npos){
                    enrollCheck = 0;
                    taCheck = 0;
                }
            }
        }
        if((*user).getStatus() == "Teaching Assistant"){
            string assistantCheck;
            assistantCheck = "<taname:" + (*user).getName() + ">";
            while(getline(course,line)){
                if(line.find(assistantCheck) != string::npos){
                    pos = line.find("<coursecode:") + 12;
                    pos2 = line.find(">",pos);
                    courseCode = line.substr(pos, pos2 - pos);
                    pos = line.find("<coursename:") + 12;
                    pos2 = line.find(">",pos);
                    courseName = line.substr(pos, pos2 - pos);
                    cout << endl << x << ") (" << courseCode << ") " << courseName;
                    x++;
                }
            }

        }
        if((*user).getStatus() == "Professor"){
            string professorCheck;
            professorCheck = "<professorname:" + (*user).getName() + ">";
            while(getline(course,line)){
                if(line.find(professorCheck) != string::npos){
                    pos = line.find("<coursecode:") + 12;
                    pos2 = line.find(">",pos);
                    courseCode = line.substr(pos, pos2 - pos);
                    pos = line.find("<coursename:") + 12;
                    pos2 = line.find(">",pos);
                    courseName = line.substr(pos, pos2 - pos);
                    cout << endl << x << ") (" << courseCode << ") " << courseName;
                    x++;
                }
            }
        }
        
        course.close();
    }
    return x; //Returns the number of the courses that are printed. When students are enrolling to a course if there are no courses printed, they won't be allowed to type any course name since there are no courses available to enroll.
}

void seeGrade(userData* user, courseData* courseInfo){
    string courseCode, courseName, studentName, grade, line;
    int inTheCourse = 0;
    int ifStudent = 0;
    size_t pos = 0;
    size_t pos2 = 0;
    cout << "*************************************************************";
    if((*user).getStatus() == "Student"){
        ifstream grades("course_info.txt");
        if(grades.is_open()){
            while(getline(grades,line)){
                if(line.find("<coursecode:") != string::npos){
                    inTheCourse = 1;
                    pos = line.find("<coursecode:") + 12;
                    pos2 = line.find(">",pos);
                    courseCode = line.substr(pos, pos2 - pos);
                    pos = line.find("<coursename:") + 12;
                    pos2 = line.find(">",pos);
                    courseName = line.substr(pos, pos2 - pos);
                }
                if(line.find("<studentname:" + (*user).getName() + ">") != string::npos){
                    ifStudent = 1;
                    pos = line.find("<grade:") + 7;
                    pos2 = line.find(">",pos);
                    if(pos != pos2)
                        grade = line.substr(pos, pos2 - pos);
                    else
                        grade = "";
                }
                if(line.find("<endofcourse>") != string::npos){
                    inTheCourse = 0;
                    if(grade != ""){
                        cout << endl << "(" << courseCode << ") " << courseName << " : " << grade;
                        grade = "";
                        ifStudent = 0;
                    }
                    else if(grade == "" && ifStudent == 1){
                        cout << endl << "(" << courseCode << ") " << courseName << " : Not Graded";
                        ifStudent = 0;
                    }
                }
            }
            grades.close();
            if((*user).getGpa() <= 4 && (*user).getGpa() >= 0){
                cout << endl << "Gpa : " << (*user).getGpa();
            }
        }
    }
    if((*user).getStatus() == "Teaching Assistant" || (*user).getStatus() == "Professor"){
        int inTheCourse = 0;
        string statusCheck;
        if((*user).getStatus() == "Teaching Assistant")
            statusCheck = "<taname:" + (*user).getName() + ">";
        if((*user).getStatus() == "Professor")
            statusCheck = "<professorname:" + (*user).getName() + ">";
        seeCourses(user,0);
        int courseCheck;
        cout << endl << "Type the course code of the course to see the students' grades : ";
        getline(cin,courseCode);
        courseCode = upperCaseName(courseCode);
        do{
            courseCheck = (*courseInfo).setCourseCode(courseCode);
            if(courseCheck == 0 && (*courseInfo).getStudentCount() != 0){
                cout << endl << "Grades of all the students in " << (*courseInfo).getCourseName() << " :" << endl;
                for(int i = 0; i < (*courseInfo).getStudentCount(); i++){
                    cout << endl << (*courseInfo).getStudentGrades()[i][0];
                }
            }
            if(courseCheck == 0 && (*courseInfo).getStudentCount() == 0){
                cout << endl << "There are no students in this class. You are being sent back to the previous menu.";
            }
            if(courseCheck == 1){
                cout << endl << "Incorrect input! Please type the course code of the course again to see the students' grades : ";
                getline(cin,courseCode);
                courseCode = upperCaseName(courseCode);
                (*courseInfo).clearCourse();
            }
        }while(courseCheck == 1);
    }
    cout << endl << "*************************************************************" << endl << endl;
}

void changeGrade(userData* user, courseData* courseInfo){
    string courseCode, studentName, line;
    int inTheCourse = 0;
    int studentExist = 0;
    if((*user).getStatus() != "Student"){
        seeGrade(user, courseInfo);
        courseCode = (*courseInfo).getCourseCode();
        if((*courseInfo).getStudentCount() != 0){
            cout << endl << "*************************************************************";
            cout << endl << "Choose a student to change the grade : ";
            getline(cin,studentName);
            studentName = fullUpperCaseName(studentName);
            while(studentExist == 0){
                for(int i = 0; i < (*courseInfo).getStudentCount(); i++){
                    if((*courseInfo).getStudentGrades()[i][0].find(studentName) != string::npos){
                        studentExist = 1;
                    }
                }
                if(studentExist == 0){
                    cout << endl << "There are no student whose name is " << studentName << " in this course. Please choose another student : ";
                    getline(cin,studentName);
                    studentName = fullUpperCaseName(studentName);
                }
            }
            ifstream course("course_info.txt");
            ofstream temp("temp.txt");
            if(course.is_open() && temp.is_open()){
                while(getline(course,line)){
                    if(line.find(courseCode) != string::npos){
                        inTheCourse = 1;
                    }
                    if(line.find("<studentname:" + studentName + ">") != string::npos && inTheCourse == 1){
                        studentExist = 1;
                        size_t pos = 0;
                        size_t pos2 = 0;
                        string grade;
                        pos = line.find("<grade:") + 7;
                        pos2 = line.find(">",pos);
                        if(pos == pos2){
                            cout << endl << studentName << " is not graded yet.";
                        }
                        else{
                            grade = line.substr(pos, pos2 - pos);
                            cout << endl << "The grade of " << studentName << " is : " << grade;
                        }
                        do{
                            cout << endl << "Type the new letter grade of " << studentName << " : ";
                            getline(cin,grade);
                            size_t letterGrade = 0;
                            for(char c : grade){
                                if(c <= 'z' && c >= 'a'){
                                    char c1 = c - ('z' - 'Z');
                                    string tempC = "";
                                    tempC = tempC + c1;
                                    letterGrade = grade.find(c, letterGrade);
                                    grade.replace(letterGrade, 1, tempC);
                                    letterGrade = letterGrade + 1;
                                }
                            }
                            if(grade != "FF" && grade != "DD" && grade != "DC" && grade != "CC" && grade != "CB" && grade != "BB" && grade != "BA" && grade != "AA")
                                cout << endl << "Incorrect input." << endl << "Please type one of the following letter grades : 'FF' , 'DD' , 'DC' , 'CC' , 'CB' , 'BB', 'BA' , 'AA'";
                        }while(grade != "FF" && grade != "DD" && grade != "DC" && grade != "CC" && grade != "CB" && grade != "BB" && grade != "BA" && grade != "AA");
                        if(pos == pos2)
                            line.insert(pos,grade);
                        else
                            line.replace(pos, 2, grade);
                    }
                    if(line.find("<endofcourse>") != string::npos && inTheCourse == 1){
                        inTheCourse = 0;
                    }
                    temp << line << endl;
                }
                temp.close();
                course.close();
                remove("course_info.txt");
                rename("temp.txt", "course_info.txt");
            }
            cout << "*************************************************************" << endl << endl;
        }
    }
    (*courseInfo).clearCourse();
}


void dropCourse(userData* user, courseData* courseInfo){
    string courseCode,line;
    int inTheCourse, incorrectSelection = 1;
    int notStudent = 1;
    cout << endl << "*************************************************************";
    seeCourses(user, 1);
    string selection;
    do{
        cout << endl << "Type the course code of the course you want to drop : ";
        do{
            getline(cin,courseCode);
            courseCode = upperCaseName(courseCode);
            incorrectSelection = (*courseInfo).setCourseCode(courseCode);
            if(incorrectSelection == 1){
                cout << endl << "Incorrect input! Type 1 to type the course code again. Type 2 to go back to previous menu : ";
                getline(cin,selection);
                while(selection != "1" && selection != "2"){
                    cout << endl << "Incorrect input! Please type 1 or 2 : ";
                    getline(cin,selection);
                }
                if(selection == "1"){
                    cout << endl << "Type the course code of the course you want to drop : ";
                }
                if(selection == "2"){
                    incorrectSelection == 0;
                }
            }
        }while(incorrectSelection == 1);
        if(selection != "2"){
            ifstream course("course_info.txt");
            ofstream temp("temp.txt");
            if(course.is_open() && temp.is_open()){
                while(getline(course,line)){
                    if(line.find("<coursecode:" + courseCode + ">") != string::npos){
                        inTheCourse = 1;
                        temp << line << endl;
                    }
                    else if(inTheCourse == 1 && line.find("<studentname:" + (*user).getName() + ">") != string::npos){
                        notStudent = 0;
                    }
                    else if(inTheCourse == 1 && line.find("<endofcourse>") != string::npos){
                        inTheCourse = 0;
                        temp << line << endl;
                    }
                    else{
                        temp << line << endl;
                    }
                }
                course.close();
                temp.close();
                remove("course_info.txt");
                rename("temp.txt","course_info.txt");
                if(notStudent == 1){
                    cout << endl << "Incorrect input! Type 1 to type the course code again. Type 2 to go back to previous menu : ";
                    getline(cin,selection);
                    while(selection != "1" && selection != "2"){
                        cout << endl << "Incorrect input! Please type 1 or 2 : ";
                        getline(cin,selection);
                    }
                    if(selection == "2"){
                        notStudent == 1;
                    }
                }
            }
            (*user).setGpa();
        }
        (*courseInfo).clearCourse();
    }while(notStudent == 1 && selection != "2");
    cout << endl << "*************************************************************" << endl << endl;
}

string encode(string password){
    int length = password.length();
    string encodedPassword = "";
    for(int i = 0; i < length; i++){
        encodedPassword += password[i] - length;
    }
    return encodedPassword;
}
