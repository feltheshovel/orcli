#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) {
    //IMPORTANT: path to used .ork file (must modify manually)
    const std::string FILEPATH = "C:/Users/Fshan/OneDrive/Documents/Felix/code/orcli/FH9_nominal_R05.ork";
    const std::string AHKPATH = "C:/\"Program Files\"/AutoHotkey/v2/AutoHotkey.exe";

    //make sure the number of arguments is correct
    if(argc != 6) {
        std::cerr << "wrong number of arguments" << std::endl;
        std::cout << "./orcli <component> <property> <start> <end> <iterations>" << std::endl;
        return 1;
    }

    //give comprehensible names to arguments
    std::string comp = argv[1];
    std::string prop = argv[2];
    double start = std::stod(argv[3]);
    double end = std::stod(argv[4]);
    int n = std::stoi(argv[5]);
    double inc = (end-start)/(n-1);  //increment between each simulation
    //variable to store the current value of <property> during each simulation
    double val = start;
    
    //main loop; each iteration is one simulation run
    for(int k = 0 ; k < n ; k++) {
        //unzip the .ork file using a python program
        //it extracts "rocket.ork", a .xml file we can modify
        std::string t = "python unzip.py " + FILEPATH;
        system(t.c_str());

        //modify <property>'s value to "val"
        /*
        here, we will copy the contents of "rocket.ork" into a new "rocket.ork"
        file in order to */
        //rename "rocket.ork" to a temporary file
        if(rename("rocket.ork","temp.ork")) {  //check if successful
            std::cerr << "Error: failed to rename \"rocket.ork\" to \"temp.ork\"" << std::endl;
            return 1;
        }
        //open "temp.ork" for reading
        std::ifstream temp("temp.ork");
        if(!temp.is_open()) {  //check if open
            std::cerr << "Error: failed to open temp.ork" << std::endl;
            return 1;
        }
        //create a new "rocket.ork" file, open it for writing
        std::ofstream rocket("rocket.ork");
        if(!rocket.is_open()) {  //check if open
            std::cerr << "Error: failed to open rocket.ork" << std::endl;
            return 1;
        }
        //read "rocket.ork" line by line until <component> and <property> are found
        std::string buffer;
        bool success = 0;  //whether "comp" and "prop" have been found
        //copying line by line until "comp" is found
        for(;;) {
            getline(temp,buffer);
            rocket << buffer;  //copy line into "rocket.ork"
            if(temp.eof()) {  //if the end is reached
                break;
            }
            rocket << std::endl;
            size_t t = buffer.find(comp);
            if(t < buffer.size()) {  //if "comp" is found
                success = 1;
                break;
            }
        }
        if(!success) {  //if "comp" is not found
            rocket.close();
            std::cerr << "Error: could not find component" << std::endl;
            return 1;
        }
        success = 0;
        //copying line by line until "prop" is found
        for(;;) {
            getline(temp,buffer);
            size_t t = buffer.find(prop);
            if(t < buffer.size()) {  //if "prop" is found
                success = 1;
                t += prop.length();  //position after "prop" in "rocket.ork"
                //assume "prop" is wrong if the character after it is not '>'
                if(buffer[t] != '>') {
                    std::cerr << "Error: could not find property" << std::endl;
                    return 1;
                }
                t++;  //position after the '>'; first character of the current value
                for(int i = 0 ; i < buffer.length() ; i++) {
                    //next '<' marks end of current value
                    if(buffer[t + i] == '<') {
                    //if true, i is the number of characters of current value
                        //replace current value with "val"
                        buffer.replace(t,i,std::to_string(val));
                        //success message for me
                        std::cout << "Modified " << prop << " of " << comp
                        << " to " << val << std::endl;
                        std::cout << buffer << std::endl;
                        break;
                    }
                }
            }
            rocket << buffer;  //copy buffer into "rocket.ork"
            if(temp.eof() || success) {  //if the end is reached or <property> modified
                break;
            }
            rocket << std::endl;
        }
        if(!success) {  //if "prop" is not found
            std::cerr << "Error: could not find property" << std::endl;
            return 1;
        }
        //copy the rest of the file
        for(;;) {
            getline(temp,buffer);
            rocket << buffer;  //copy line into "rocket.ork"
            if(temp.eof()) {  //if the end is reached
                break;
            }
            rocket << std::endl;
        }
        //close files
        temp.close();
        rocket.close();
        remove("temp.ork");
        
        //run the simulation with the modified file
        //zip using a python program
        t = "python zip.py " + FILEPATH;
        system(t.c_str());
        //open the zipped file in OR using AHK
        t = AHKPATH + " open.ahk " + FILEPATH;
        system(t.c_str());
        //run the simulation using AHK
        t = AHKPATH + " run.ahk";
        system(t.c_str());
        //save the simulation results using AHK
        t = AHKPATH + " save.ahk";
        system(t.c_str());

        //enter the simulation results into "results.txt"
        //unzip the .ork file using a python program
        t = "python unzip.py " + FILEPATH;
        system(t.c_str());
        //create an "ifstream" object to read "rocket.ork"
        std::ifstream rocket1("rocket.ork");
        if(!rocket1.is_open()) {
            std::cerr << "Error: failed opening \"rocket.ork\"" << std::endl;
            return 1;
        }
        //create an "ofstream" object to write to "results.txt"
        std::ofstream results;
        if(!k)  {  //if it is the first simulation
            // open with the "trunc" flag to delete previous contents
            results.open("results.txt",std::ios::trunc);
        } else {  //if it is not the first simulation
            //open it with "app" flag to append to file end
            results.open("results.txt",std::ios::app);
        }
        if(!results.is_open()) {
            std::cerr << "Error: failed opening \"results.txt\"" << std::endl;
            return 1;
        }
        //read "rocket.ork" line by line until the results are found
        for(;;) {
            getline(rocket1,buffer);
            if(rocket1.eof()) {  //if the end is reached
                break;
            }
            //the line containing the results also contains the string "maxaltitude"
            size_t t = buffer.find("maxaltitude");
            if(t < buffer.size()) {  //if "maxaltitude" is found
                //copy its line to "results.txt"
                results << "Simulation " << k << ":\n" << buffer << "\n\n";
                break;
            }
        }
        //close files
        rocket1.close();
        results.close();
                
        val += inc;  //increment "val" by "inc"
    }

    remove("rocket.ork");  //delete "rocket.ork" to clean up

    return 0;
}