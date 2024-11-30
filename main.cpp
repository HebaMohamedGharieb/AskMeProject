#include <iostream>
#include<fstream>
#include <sstream>
#include <vector>
using namespace std;
string usersFilePath = "C:\\Users\\Heba\\CLionProjects\\ASKMeV1\\users.txt";
ifstream usersFileBinary("C:\\Users\\Heba\\CLionProjects\\ASKMeV1\\users.txt", std::ios::binary | std::ios::ate);

string questionFilePath = "C:\\Users\\Heba\\CLionProjects\\ASKMeV1\\questions.txt";
ifstream questionFileBinary("C:\\Users\\Heba\\CLionProjects\\ASKMeV1\\questions.txt", std::ios::binary | std::ios::ate);

int getTheLastID2(fstream& file){
    int id = 0;
    string line, lastLine;

    if (file.is_open()) {
        while (getline(file, line)) {
            lastLine = line;
        }
        if (!lastLine.empty()) {
            istringstream iss(lastLine);
            iss >> id;
        }
    }
    return id + 1;
}

vector<vector<string>> read_file_segments(fstream&  file){
    string line;

    vector<vector<string>> lines;
    if (file.is_open()){
        while (getline(file, line)){
            vector<string> segments ;
            stringstream ss(line);
            string segment;
            while (getline(ss, segment, ',')){
                segments.push_back(segment);

            }
            lines.push_back(segments);
        }
    }else{
        cout<<"the file can not open from read_file_segments function!! \n";
    }
    return lines;
}

void printQuestionsToTheUser (int current_user_id, const string& current_user_name, string mail)
{

    vector<vector<string>> lines;
    fstream questionFile(questionFilePath.c_str(), ios::out | ios::in | ios::app);
    if(questionFile.is_open()){
        lines = read_file_segments(questionFile);
        for (int i = 0; i < lines.size(); ++i) {
            int question_id=0, reply_on=0, who_ask_id=0, who_asked_id=0, thread_to_question_id=0;

            stringstream(lines[i][0])>>question_id, stringstream (lines[i][1])>>reply_on,
                    stringstream (lines[i][2])>>who_ask_id, stringstream(lines[i][3])>> who_asked_id;
            thread_to_question_id = question_id;

            if (current_user_id == who_asked_id && reply_on == -1){
                cout<<"Question id("<<question_id<<") from user id("<<who_ask_id<<"):       Question: "<< lines[i][4]<<"\n";
                if (lines[i].size() == 6){
                    cout<<"        Answer: "<<lines[i][5]<<"\n";
                }
                for (int j = 0; j < lines.size(); ++j) {

                    stringstream(lines[j][0])>>question_id, stringstream (lines[j][1])>>reply_on,
                            stringstream (lines[j][2])>>who_ask_id, stringstream(lines[j][3])>> who_asked_id;

                    if (current_user_id == who_asked_id && reply_on != -1 && reply_on==thread_to_question_id){
                        cout<<"        thread: "<<"Question id("<<question_id<<") from user id("<<who_ask_id<<"):       Question: "<< lines[j][4]<<"\n";
                        if (lines[j].size()==6)
                            cout<<"        thread:          "<<"Answer: "<< lines[j][5]<<"\n";
                    }
                }
            }
        }
    }
}

void printQuestionFromTheUser(int current_user_id, const string& current_user_name, string mail)
{
    vector<vector<string>> lines;
    fstream questionFile(questionFilePath.c_str(), ios::out | ios::in | ios::app);
    if(questionFile.is_open()){
        lines = read_file_segments(questionFile);
        for (int i = 0; i < lines.size(); ++i) {
            int question_id=0, reply_on=0, from_user_id=0, to_user_id=0, thread_to_question_id=0;

            stringstream(lines[i][0])>>question_id, stringstream (lines[i][1])>>reply_on,
                    stringstream (lines[i][2])>>from_user_id, stringstream(lines[i][3])>> to_user_id;
            thread_to_question_id = question_id;

            if (from_user_id==current_user_id){
                if (lines[i].size() == 6)
                    cout<<"Question id("<<question_id<<") to user id("<<to_user_id<<"):     Question: "<< lines[i][4]<<"      Answer: "<<lines[i][5]<<"\n";
                else
                    cout<<"Question id("<<question_id<<") to user id("<<to_user_id<<"):     Question: "<< lines[i][4]<<"      Not Answered Yet.\n";
                cout<<"\n";
            }
        }
    }
}

void answerQuestion(int current_user_id, const string& current_user_name, string mail)
{
    int answering_question_id = 0;
    bool find_question = false;
    string answer;
    vector<vector<string>> lines, updated_lines;

    cout<<"enter the id of the question that you want to answer: ";
    cin>>answering_question_id;
    cout<<"\n";

    fstream questionFile(questionFilePath.c_str(), ios::out | ios::in );

    if (questionFile.is_open())
    {
        lines = read_file_segments(questionFile);
        for (auto & line : lines)
        {
            int question_id=0, reply_on=0, who_ask_id=0, who_asked_id=0, thread_to_question_id=0;

            stringstream (line[0])>>question_id;
            stringstream (line[1])>>reply_on;
            stringstream (line[2])>>who_ask_id;
            stringstream (line[3])>>who_asked_id;

            if(question_id == answering_question_id){
                find_question = true;
                if (line.size() == 5)
                {
                    cout<<"question id ("<<question_id<<") from user id("<<who_ask_id<<"):      "<<"Question: "<<line[4]<<"\n";
                    cout<<"Enter your answer: ";
                    cin.ignore();
                    getline(cin, answer);
                    cout<<"\n";
                    line.push_back(answer);


                }
                else if(line.size() == 6)
                {
                    cout<<"question id ("<<question_id<<") from user id("<<who_ask_id<<"):      "<<"Question: "<<line[4];
                    cout<<"            Answer: "<<line[5]<<"\n";
                    cout<<"Warning: This question is already answered, the answer will be updated.";
                    cout<<"Enter your answer: ";
                    cin.ignore();
                    getline(cin, answer);
                    cout<<"\n";
                    line[5] = answer;
                }
            }
        }
        questionFile.close();
    }
    else
    {
        cerr<<"Error opening the file.\n";
    }
    if (find_question)
    {
        fstream questionFileOut(questionFilePath.c_str(), ios::out | ios::trunc);
        if (questionFileOut.is_open())
        {
            for (const auto& line : lines) {
                for (const auto& segment : line) {
                    questionFileOut<<segment<<",";
                }
                questionFileOut<<"\n";
            }
            cout<<"Question had answered successfully.\n";
        }
        else
        {
            cerr<<"Error opening the file write.\n";
        }
    }
}

void deleteQuestion(int current_user_id, const string& current_user_name, string mail){

    int delete_question_id = 0;
    bool find_question = false;
    vector<vector<string>> lines, updated_lines;
    fstream questionFile(questionFilePath.c_str(), ios::out | ios::in );

    cout<<"enter the id of the question that you want to delete: "; cin>>delete_question_id; cout<<"\n";

    if(questionFile.is_open())
    {
        lines = read_file_segments(questionFile);

        for (auto & line : lines)
        {

            int question_id=0, reply_on=0, who_ask_id=0, who_asked_id=0, thread_to_question_id=0;

            stringstream(line[0])>>question_id;
            stringstream (line[1])>>reply_on;
            stringstream (line[2])>>who_ask_id;
            stringstream(line[3])>> who_asked_id;

            if (delete_question_id == question_id && current_user_id != who_asked_id)
            {
                cout<<"you are not allowed to delete this question, please try to delete your own questions\n";
                //call menu here
                return;
            }

            if (!(delete_question_id == question_id || reply_on == delete_question_id))
            {
                updated_lines.push_back(line);
                find_question = true;
            }

        }
        questionFile.close();
    }
    else
    {
        cerr << "Error opening the file for writing.\n";
    }
    if (!find_question)
    {
        cout<<"There is not a question with this id, please try again.\n";
        //calling menu here
    }
    else
    {
        fstream questionFileOut(questionFilePath.c_str(), ios::out | ios::trunc);
        if (questionFileOut.is_open())
        {
            for (const auto& line : updated_lines)
            {
                for (const auto& segment : line)
                {
                    questionFileOut << segment << ",";
                }
                questionFileOut << "\n";
            }
            questionFileOut.close();
            cout << "Question deleted successfully.\n";
        }
        else
        {
            cerr << "Error opening the file for writing.\n";
        }
    }

}

void askQuestion(int current_user_id, const string& current_user_name, string mail){
    int reply_on, question_id=0, asked_user_id=0;
    string question;

    fstream questionFileRead(questionFilePath.c_str(), ios::in);
    fstream questionFileWrite(questionFilePath.c_str(), ios::out | ios::app);


    if (!questionFileRead.is_open() || !questionFileWrite.is_open()) {
        cout << "Error... Files cannot be opened!\n";
        return;
    }
    cout<<"Enter user id or -1 to cancel: "; cin>> asked_user_id;
    if (asked_user_id != -1)
    {

        cout<< "\n" << "for a thread question enter the question id for new question enter -1: "; cin >> reply_on; cin.ignore();
        cout<< "\n" << "Enter your question text: "; getline(cin, question);

        question_id = getTheLastID2(questionFileRead);
////    question_id,  reply on? ,  who_ask, who_is_asked, question, answer.
        questionFileWrite<< question_id<< "," << reply_on << "," << current_user_id << "," << asked_user_id << "," << question << "," <<"\n";
    }
    questionFileRead.close();
    questionFileWrite.close();

}

void listSystemUser(int current_user_id, const string& current_user_name, string mail){
    string line, each_user_line;

    string name, user_name, password;
    int user_id;

    fstream usersFile(usersFilePath.c_str(), ios::out | ios::in | ios::app);
    if(usersFile.is_open()){
        usersFileBinary.seekg(0, ios::beg);
        if (usersFileBinary.peek() != ifstream::traits_type::eof()){
            while(getline(usersFile, line)){
                each_user_line = line;
                istringstream  iss(each_user_line);
                iss >> user_id >> user_name >> password >> name;
                cout<< "User ID: " << user_id << "      " << "User Name: " << name <<"\n";
            }
        }
    }
    usersFile.close();
}

void logOut();
void feed(){}

void menu(int userID, string name, string mail){

    cout<<"Hi "<< name << " \n" << "E-Mail: " << mail << " \n" << "ID: "<<userID<<"\n\n";

    int choice = 0;
    cout<<"menu: \n\n";
    cout<<"     "<<"1) print questions to the user. \n";
    cout<<"     "<<"2) print question from the user. \n";
    cout<<"     "<<"3) answer question. \n";
    cout<<"     "<<"4) delete question. \n";
    cout<<"     "<<"5) ask question. \n";
    cout<<"     "<<"6) list system user. \n";
    cout<<"     "<<"7) feed. \n";
    cout<<"     "<<"8) logout. \n";

    cout<< "enter number in range 1-8: \n";
    cin>>choice;
    if(choice >= 1 && choice <=8){
        switch (choice) {
            case 1:
                printQuestionsToTheUser(userID, name, mail);
                menu(userID, name, mail);
                break;
            case 2:
                printQuestionFromTheUser(userID, name, mail);
                menu(userID, name, mail);
                break;
            case 3:
                answerQuestion(userID, name, mail);
                menu(userID, name, mail);
                break;
            case 4:
                deleteQuestion(userID, name, mail);
                menu(userID, name, mail);
                break;
            case 5:
                askQuestion(userID, name, mail);
                menu(userID, name, mail);
                break;
            case 6:
                listSystemUser(userID, name, mail);
                menu(userID, name, mail);
                break;
            case 7:
                feed();
                menu(userID, name, mail);
                break;
            case 8:
                logOut();
                menu(userID, name, mail);
                break;
        }
    }else{
        cout<<"ERROR!! invalid number ... try again: \n";
        cin>>choice;
        while (choice < 1 || choice > 8){
            cout<<"ERROR!! invalid number ... try again: \n";
            cin>>choice;
        }
    }
}

void signup(){
    string name, user_name, user_password, user_email;
    int user_id = 0;
    fstream usersFileRead(usersFilePath.c_str(), ios::in);
    fstream usersFileWrite(usersFilePath.c_str(), ios::out | ios::app);

    if (!usersFileRead.is_open() || !usersFileWrite.is_open()){
        cout << "Error... Files cannot be opened!\n";
        return;
    }

    user_id = getTheLastID2(usersFileRead);
    cout<<"please Enter user name, password, full name and your email \n";
    cin>>user_name>>user_password>>name>>user_email;
    usersFileWrite<<user_id<<" "<<user_name<<" "<<user_password<<" "<<name<<" "<<user_email<<"\n";
    menu(user_id, user_name, user_email);

    usersFileRead.close();
    usersFileWrite.close();

}

void logIn(){
    bool check_if_the_user_have_account = false;
    char check_if_the_user_want_to_register;
    string stored_user_name, stored_password, stored_name, stored_email, user_name, password;
    int stored_user_id;
    cout<<"please enter your user name and password: \n";
    cin>>user_name>>password;
    fstream usersFile(usersFilePath.c_str(), ios::out | ios::in | ios::app);
    string line, user_data_line;
    if(usersFile.is_open()){
        usersFileBinary.seekg(0, ios::beg);
        if (usersFileBinary.peek() != ifstream::traits_type::eof()) {
            while (getline(usersFile, line)){
                user_data_line=line;
                istringstream iss(user_data_line);
                iss>>stored_user_id>>stored_user_name>>stored_password>>stored_name>>stored_email;
                if (user_name == stored_user_name && password == stored_password){
                    check_if_the_user_have_account = true;
                    menu(stored_user_id, stored_name, stored_email);
                    break;
                }else if (user_name == stored_user_name && password != stored_password){
                    cout<<"Sorry, Your password is not correct, please try again: \n";
                    cout<<"please enter your password correctly : \n";
                    while (password != stored_password){
                        cin>> password;
                    }
                    menu(stored_user_id, stored_name, stored_email);
                }
            }
        }
        if (!check_if_the_user_have_account || usersFileBinary.peek() == ifstream::traits_type::eof()){
            cout<<"Sorry, You do not have an account!! if you want to sign up enter (y) to cancel enter(n)  \n";
            cin>>check_if_the_user_want_to_register;
            if (check_if_the_user_want_to_register == 'y' || check_if_the_user_want_to_register == 'Y')
                signup();

            else cout<<"exit \n";
        }
    }
    usersFile.close();

}

void authenticationMenu(){
    int choice = 0;
    cout<<"menu: \n";
    cout<<"     "<<"1) login. \n";
    cout<<"     "<<"2) signup. \n";

    cout<< "enter number 1 or 2: \n";
    cin>>choice;

    while (!(choice == 1 || choice == 2)){
        cout<<"ERROR!! invalid number ... try again: \n";
        cin>>choice;
    }
    switch (choice) {
        case 1:
            logIn();
            break;
        case 2:
            signup();
            break;
    }
}
void logOut(){
    authenticationMenu();
}

int main() {
    authenticationMenu();
    return 0;
}

