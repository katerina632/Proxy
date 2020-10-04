#include <iostream>
#include <string>
#include <fstream>
#include<ctime>

using namespace std;

class Database {
private:
	const string  connectionString;
	bool isConnect;
public:
	Database() : connectionString("database.txt"), isConnect(false) {}
	   
	virtual bool Connect(string strConn)
	{
		bool IsConnectedImmitatingConnect = (rand() % 10) < 6;

		if (strConn == connectionString)
		{
			isConnect = IsConnectedImmitatingConnect;
		}
			return isConnect;
		
	}
	virtual void Log(string msg)
	{
		if (isConnect)
		{
			ofstream fout;
			fout.open(connectionString, ofstream::app);
			fout << msg << endl;
			fout.close();
			cout << "Writing in database:" << msg << endl;
		}	
	};

	

};

class DatabaseProxy : public Database {
private:
	Database* realDB = new Database();
	bool isConnect;
public:	

	bool Connect(string strConn) override
	{
		isConnect = realDB->Connect(strConn);
		return isConnect;
	}

	void Log(string msg) override 
	{		

		if (!isConnect)
		{
			ofstream localFout;

			localFout.open("localTempDatabase.txt", ofstream::app);
			if (localFout.is_open())
			{
				localFout << msg << endl;
				localFout.close();
			}
			else
			{
				throw exception("Unable to open file.\n");
			}
			
		}
		else 
		{
			ifstream localFin("localTempDatabase.txt");
			string temp;

			if (localFin.is_open()) 
			{
				while (!localFin.eof()) {
					if (getline(localFin, temp))
						realDB->Log(temp);
				}

				localFin.close();

				ofstream clear("localTempDatabase.txt", ios::trunc);
				clear.close();
				realDB->Log(msg);
			}
			else {
				throw exception("Unable to open file.\n");
			}
			
		}
	}
};

int main()
{
	srand(unsigned(time(0)));
	try {
		DatabaseProxy* dbP = new DatabaseProxy();
		dbP->Connect("database.txt");
		dbP->Log("Some message");

		dbP->Connect("database.txt");
		dbP->Log("Some message 2");
		delete dbP;
	}
	catch (exception ex) {
		cout << ex.what() << endl;
	}

	system("pause");
	return 0;
}