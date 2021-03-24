#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <limits>
std::string GetFileContents(std::string fileName) {
	std::ifstream in(fileName);
	if (in.fail()) return "FAILED TO LOAD FROM FILE";
	std::stringstream test;
	std::string s;
	while (in.good()) {
		std::getline(in, s);
		test << s << '\n';
	}
	s = test.str();
	s.pop_back();
	in.close();
	return s;
}
void EmptyCin() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
int rdm_int(int min, int max)
{
	auto rdm = std::mt19937(std::random_device()());
	return std::uniform_int_distribution<int>(min, max)(rdm);
}
char rdm_char1() {
	return rdm_int(33, 37);
}
char rdm_char2() {
	return rdm_int(58, 64);
}
char rdm_char3() {
	return rdm_int(91, 96);
}
char rdm_char4() {
	return rdm_int(123, 126);
}
char rdm_char() {
	int n = rdm_int(1, 484) % 22;
	if (n < 5) return rdm_char1();
	else if (n < 12) return rdm_char2();
	else if (n < 18) return rdm_char3();
	else return rdm_char4();
}
std::string rdm_string(int length) {
	std::string s = "";
	for (int i = 0; i < length; ++i) {
		s.push_back(rdm_char());
	}
	return s;
}
bool isValidChar(char c) {
	if ((c >= 33 && c <= 37) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126)) {
		return true;
	}
	return false;
}
void Stage1E(std::string& msg) {
	std::reverse(msg.begin(), msg.end());
}
void Stage2E(std::string& msg) {
	std::string s = "";
	int n = 0;
	for (auto c : msg) {
		n = c << 6;
		n += 999;
		s += (std::to_string(n));
	}
	msg = s;
}
void Stage3E(std::string& msg) {
	std::string newmsg = "";
	for (int i = 0; i < (int)msg.size(); ++i) {
		if (msg[i] >= '0' && msg[i] <= '9') {
			newmsg.push_back(95 - msg[i]);
			newmsg += rdm_string(2);
		}
	}
	msg = newmsg;
}
void Stage4E(std::string& msg) {
	msg.insert(0, rdm_string(5));
	msg += rdm_string(5);
}
void Encrypt(std::string& msg) {
	//Reverse
	Stage1E(msg);
	//Bitshift ASCII values of all chars 6 spaces to the left, add 999 to that number, then replace all chars with these values
	Stage2E(msg);
	/*Convert all single number characters to a character of ASCII value of 95 minus the ASCII value of that number character
	and insert 2 random characters with specific ASCII value (check rdm_char() function*/
	Stage3E(msg);
	/*Add 5 random characters with specific ASCII value (check rdm_char() function to the front and another 5 to the end of the message
	for decryptor to check if the message is coded*/
	Stage4E(msg);
	std::cout << "\n\t\tMESSAGE ENCRYPTED!\n\n\n";
}
bool Stage1D(std::string& msg) {
	if ((int)msg.size() < 10) {
		return false;
	}
	for (int i = 0; i < 5; ++i) {
		int front = msg[0];
		int back = msg[(int)msg.size() - 1];
		if (isValidChar(msg[0]) && isValidChar(msg[(int)msg.size() - 1])) {
			msg.erase(0, 1);
			msg.pop_back();
		}
		else {
			return false;
		}
	}
	return true;
}
void Stage2D(std::string& msg) {
	for (int i = 0; i < (int)msg.size(); ++i) {
		if (msg[i] > 34 && msg[i] < 48) {
			msg[i] = 95 - msg[i];
		}
	}
}
bool Stage3D(std::string& msg) {
	std::string s = "";
	std::string newmsg = "";
	for (auto c : msg) {
		if (c >= '0' && c <= '9') {
			s.push_back(c);
		}
		else if (c == 10) {
			newmsg.push_back('\n');
		}
		else if (!isValidChar(c)) {
			return false;
		}
		if ((int)s.size() == 4) {
			newmsg.push_back((std::stoi(s) - 999) >> 6);
			s.clear();
		}
	}
	msg = newmsg;
	return true;
}
void Stage4D(std::string& msg) {
	std::reverse(msg.begin(), msg.end());
}
bool Decrypt(std::string& msg) {
	//Check if message is encrypted by msgEncrypt program and remove first and last 5 characters if it is
	if (!Stage1D(msg)) {
		return false;
	}
	//Substract all characters with ASCII value between 38 and 47 from 95 and make that the new character
	Stage2D(msg);
	/*Convert all numbers from the string to ints, make 4 digit numbers from them, substract 999 from each,
	then bitshift the result 6 spaces to the right and make a string out of ASCII characters represented by those values*/
	if (!Stage3D(msg)) {
		return false;
	}
	//Reverse the string
	Stage4D(msg);
	std::cout << "\n\t\tMESSAGE DECRYPTED!\n\n\nDecrypted message:\n\n" << msg;
	return true;
}
int main()
{
	std::string msg;
	std::string inputFileName;
	std::string outputFileName;
	std::ifstream inputFile;
	std::ofstream outputFile;
	int option;
	std::cout << "What would you like to do:\n(1) Encryption\n(2) Decryption\n(3) Quit\n";
	InputOption:
	std::cin >> option;
	EmptyCin();
	switch (option) {
	case 1:
	{
	Encryption:
		std::cout << "\n\tENCRYPTION:\n\n(1) Type a new message\n(2) Load text from a file\n(3) Quit\n";
	EncryptionOption:
		std::cin >> option;
		EmptyCin();
		switch (option) {
		case 1:
		{
			std::cout << "\n\nType a message that you want to encrypt:\n\n";
			std::getline(std::cin, msg);
			break;
		}
		case 2:
		{
			std::cout << "\n\nSpecify the file to load the text from:\n";
			std::getline(std::cin, inputFileName);
			inputFile.open(inputFileName);
			while (inputFile.fail()) {
				std::cout << "\t***ERROR*** COULD NOT FIND \"" << inputFileName << "\"!\nEnter correct path:\n";
				inputFileName.clear();
				std::getline(std::cin, inputFileName);
				if (inputFile.is_open()) {
					inputFile.close();
				}
				inputFile.open(inputFileName);
			}
			msg = GetFileContents(inputFileName);
			inputFile.close();
			break;
		}
		case 3:
		{
			return 0;
			break;
		}
		default:
		{
			std::cout << "\nInvalid option! Try again.\n";
			goto EncryptionOption;
			break;
		}
		}
		Encrypt(msg);
		std::cout << "Where would you like to save your encrypted message:\n";
		std::cin >> outputFileName;
		if (outputFile.is_open()) {
			outputFile.close();
		}
		outputFile.open(outputFileName);
		outputFile << msg;
		outputFile.close();
		std::cout << "\n\tMessage saved to \"" << outputFileName << "\".\n";
	EncryptionMoreOptions:
		std::cout << "\n\nWhat would you like to do:\n(1) Encrypt another message\n(2) Decryption\n(3) Quit\n";
		std::cin >> option;
		EmptyCin();
		switch (option) {
		case 1:
		{
			goto Encryption;
			break;
		}
		case 2:
		{
			goto Decryption;
			break;
		}
		case 3:
		{
			return 0;
			break;
		}
		default:
		{
			std::cout << "\nInvalid option! Try again:\n";
			goto EncryptionMoreOptions;
			break;
		}
		}
		return 0;
	}
	case 2:
	{
	Decryption:
		std::cout << "\n\tDECRYPTION:\n";
		std:: cout << "\nSpecify the file to load the encrypted message from:\n";
		std::getline(std::cin, inputFileName);
		inputFile.open(inputFileName);
		while (inputFile.fail()) {
			std::cout << "\t***ERROR*** COULD NOT FIND \"" << inputFileName << "\"!\nEnter correct path:\n";
		DecryptFailed:
			inputFileName.clear();
			std::getline(std::cin, inputFileName);
			if (inputFile.is_open()) {
				inputFile.close();
			}
			inputFile.open(inputFileName);
		}
		msg = GetFileContents(inputFileName);
		inputFile.close();
		if (msg.empty()) {
			std::cout << "\n\t***ERROR*** DID NOT FIND MESSAGE IN \"" << inputFileName << "\"!\nSpecify the file with the encrypted message:\n";
			goto DecryptFailed;
		}
		std::cout << "\nLoaded message:\n\n" << msg << "\n\n\n";
		if (Decrypt(msg)) {
			std::cout << "\n\n\n\nWould you like to save the message to a new file:\n(1) Save\n(2) Load another file\n(3) Encryption\n(4) Quit\n";
		DecryptionSaveOption:
			std::cin >> option;
			EmptyCin();
			switch (option) {
			case 1:
			{
				std::cout << "\n\nSpacify where to save the decrypted message:\n";
				outputFileName.clear();
				std::getline(std::cin, outputFileName);
				outputFile.open(outputFileName);
				outputFile << msg;
				outputFile.close();
			DecryptionMoreOptions:
				std::cout << "\n\tMessage saved to \"" << outputFileName << "\".\n\n\nWhat would you like to do:\n(1) Load another file\n(2) Encryption\n(3) Quit\n";
				std::cin >> option;
				EmptyCin();
				switch (option) {
				case 1:
				{
					goto Decryption;
					break;
				}
				case 2:
				{
					goto Encryption;
					break;
				}
				case 3:
				{
					return 0;
					break;
				}
				default:
				{
					std::cout << "\nInvalid option! Try again:\n";
					goto DecryptionMoreOptions;
					break;
				}
				}
				break;
			}
			case 2:
			{
				goto Decryption;
				break;
			}
			case 3:
			{
				goto Encryption;
				break;
			}
			case 4:
			{
				return 0;
				break;
			}
			default:
			{
				std::cout << "\nInvalid option! Try again:\n";
				goto DecryptionSaveOption;
				break;
			}

			}
		}
		else {
			std::cout << "\t***ERROR*** MESSAGE IS IN WRONG FORMAT!\nSpecify the file with the encrypted message:\n";
			goto DecryptFailed;
		}
		break;
	}
	case 3:
	{
		return 0;
		break;
	}
	default:
	{
		std::cout << "\nInvalid option! Try again:\n";
		goto InputOption;
		break;
	}
	}
	return 0;
}