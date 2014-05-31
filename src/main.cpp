#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <math.h>

#include "class_definitions.hpp"

#define LINESIZE 128
#define BACKSPACE 0x7f
#define VT100
#define ASCII_ESC 27

using namespace std;

struct winsize w;
int newline_offset = 0;

void console_flush(string line)
{
  //cout << "columns, in characters: " << w.ws_col << endl;
#ifdef VT100
  /* Clear completions */
  printf("%c[2K", ASCII_ESC);
  //cout << (double)((double)line.length() / (double)w.ws_col);
  int line_num = (int)ceil((double)(line.length()+newline_offset) / (double)w.ws_col);
  //cout << line_num << endl;
  for (int i=0 ; i<line_num ; i++)
  /* Go to the beginning of the previous line */
    printf("%c[1A\r", ASCII_ESC);
#endif
  
  cout << line;	
}

int getchar_silent()
{
	int ch;
	struct termios oldt, newt;
	
	/* Retrieve old terminal settings */
	tcgetattr(STDIN_FILENO, &oldt);
	
	/* Disable canonical input mode, and input character echoing. */
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	
	/* Set new terminal settings */
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
	/* Read next character, and then switch to old terminal settings. */
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	
	return ch;
}

int main(int argc, char** argv) 
{
	Trie trie;
	string word;
	string path;
	ifstream myfile;
	int next;
	string userInput = "";
	string line = "";
	Recommendations recs;
	int hasRecs = -1;
	bool caps;
	
	if (argc == 1) {
		path = getenv("HOME");
		path += "/.dict";
	} 
	else if (argc == 3 && argv[1] == string("-d"))
		path = argv[2];
	else {
		cerr << "Invalid arguments" << endl;
		exit(EXIT_FAILURE);
	}
	
	myfile.open(path);
	if (myfile.is_open()) {
		while (getline(myfile, word)) 
			trie.addWord(word);
		myfile.close();
	}
	else 
		cout << "Unable to open file" << endl; 
	
	/* Keep reading one character at a time */
	while ((next = getchar_silent()) != EOF) {
		/* Print normal characters */
		
		if (isalpha(next)) {   //if alpha
          userInput += next; //append to userInput
          putchar(next);     //print
          line += next;
          continue;
		}
		else if ( ispunct(next) || next == ' ' || next == '\n') { //if delim
          if (next == '\n')
            newline_offset += w.ws_col;
          trie.updateWord(userInput); //update/add word to trie
          putchar(next);              //print
          userInput="";               //reset userInput
          line += next;
          continue;
		}
		
		/* Treat special characters differently */
		switch(next) {
			
		case '\t':              /* Just read a tab */
			caps = true;
			/* Get terminal window size */
			ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
			
			recs = trie.getRecommendations(userInput);
			hasRecs = recs.printRecommendations();
			
			if (hasRecs == -1) 
              break;
			else 
              if (recs.getNumberOfRecommendations() == 1) {
                string suffix = recs.getRecommendation(0).substr(userInput.length());
                cout << suffix;
                userInput += suffix;
                  line += suffix;
                  break;
              }
			
			while ((next = getchar_silent()) != EOF) {
              if (isalpha(next)) {
                console_flush(line);
                if (next == '\n')
                  newline_offset += w.ws_col;
                userInput += next;
                line += next;
                putchar(next);
                break;
              }
              else if ( ispunct(next) || next == ' ' || next == '\n') {
                console_flush(line);
                trie.updateWord(userInput);
                putchar(next);
                line += next;
                userInput="";
                break;
              }
              else if (isdigit(next)) {
                console_flush(line);
                int selection = next-'0';
                if (selection > 0 && selection <= recs.getNumberOfRecommendations()) {
                  if (userInput.length() > 1) {
                    for(char& c : userInput)
                      if (islower(c)) {
                        caps = false;
                        break;
                      }
                  }
                  else 
                    caps = false;
                  
                  string suffix = recs.getRecommendation(selection-1).substr(userInput.length());
                  if (caps == true)
                    transform(suffix.begin(), suffix.end(), suffix.begin(), ::toupper);
                  cout << suffix;
                  userInput += suffix;
                  line += suffix;
                  break;
                }
                else 
                  continue;
              }
			}
			break;
            
          case BACKSPACE:         /* Just read a backspace */
			
            /* Move the cursor one step back, print space, and move
             * cursor back once more to imitate backward character
             * deletion.
             */
            if (userInput.length() > 0)
              userInput.erase(userInput.end() - 1);
            if (line.length() > 0)
              line.erase(line.end() - 1);
            printf("\b \b");
            break;
            
          case VEOF:              /* Just read EOF (Ctrl-D) */
			
            /* The EOF character is recognized only in canonical input
             * mode. In noncanonical input mode, all input control
             * characters such as Ctrl-D are passed to the application
             * program exactly as typed. Thus, Ctrl-D now produces
             * this character instead.
             */
            
            printf("\n\nExiting. Bye...");
            goto THE_END;
          default:
            continue;
        }
    }
THE_END:
	ofstream ofs;
	ofs.open(path, ofstream::out | ofstream::trunc);
	trie.printTrie(ofs);
	ofs.close();
	
    putchar('\n');
    return EXIT_SUCCESS;
	return 0;	
}
