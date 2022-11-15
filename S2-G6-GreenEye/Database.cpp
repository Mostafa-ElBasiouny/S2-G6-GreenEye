#include "Database.h"

Database::Database(string filename) {
	// if file exists, open and read record.
	// else create file and initialize a new instance of record.

	// example record initialization.
	// a record will be created alongside a file and saved to it.
	// every time the record is altered it** should** be reflected in the database.
	this->Record = {};

	// update_record();

	Record dummyrec;
	dummyrec->temperature.insetr(60.0f);
	this->Record = dummyrec;
	fout << this->Record;

	fill_n(this->Record.ph, 100, 0.0f);
	fill_n(this->Record.co2, 100, 0.0f);
	fill_n(this->Record.uv_index, 100, 0.0f);
	fill_n(this->Record.temperature, 100, 0.0f);
	fill_n(this->Record.air_humidity, 100, 0.0f);
	fill_n(this->Record.precipitation, 100, 0.0f);
	fill_n(this->Record.soil_moisture, 100, 0.0f);
	fill_n(this->Record.soil_fertility, 100, 0.0f);
}

bool Database::openFile(string filename)
{
	fstream dataFile;
	dataFile.open(fileName, fstream::in | fstream::out | fstream::app);

	if (!dataFile.is_open()) {
		ifstream dataFile(fileName)
			dataFile.open(fileName, fstream::in | fstream::out | fstream::app);
	}
	if (dataFile.is_open()) {
		return 0;
	}
	else
	{
		cerr << "Error Opening/Creating Database File" << endl;
		return 1;
	}
}

//
ofstream& operator <<(ofstream& fout, Record& recco)
{
	fout << "ph:";
	for (auto& rec : recco.ph) fout << rec << ",";

	fout << endl << "co2:";
	for (auto& rec : recco.co2) fout << rec << ",";

	fout << endl << "uv_index:";
	for (auto& rec : recco.uv_index) fout << rec << ",";

	fout << endl << "temperature:";
	for (auto& rec : recco.temperature) fout << rec << ",";

	fout << endl << "air_humidity:";
	for (auto& rec : recco.air_humidity) fout << rec << ",";

	fout << endl << "precipitation:";
	for (auto& rec : recco.precipitation) fout << rec << ",";

	fout << endl << "soil_moisture:";
	for (auto& rec : recco.soil_moisture) fout << rec << ",";

	fout << endl << "soil_fertility:";
	for (auto& rec : recco.soil_fertility) fout << rec << ",";

	fout << endl << "events:";
	for (auto& rec : recco.events)
		fout << rec.timestamp << "," << rec.level << "," << rec.sensor << "," << rec.message << ",";

	fout << endl;
	fout << "ac:";
	fout << recco.ac << "," << endl;
	fout << "uv_light:";
	fout << recco.uv_light << "," << endl;
	fout << "humidifier:";
	fout << recco.humidifier << "," << endl;
	fout << "ph_regulator:";
	fout << recco.ph_regulator << "," << endl;
	fout << "water_sprinkler:";
	fout << recco.water_sprinkler << "," << endl;

	fout << "ph_range:";
	fout << recco.ph_range.first << "," << recco.ph_range.second << "," << endl;
	fout << "co2_range:";
	fout << recco.co2_range.first << "," << recco.co2_range.second << "," << endl;
	fout << "uv_index_range:";
	fout << recco.uv_index_range.first << "," << recco.uv_index_range.second << "," << endl;
	fout << "temperature_range:";
	fout << recco.temperature_range.first << "," << recco.temperature_range.second << "," << endl;
	fout << "air_humidity_range:";
	fout << recco.air_humidity_range.first << "," << recco.air_humidity_range.second << "," << endl;
	fout << "precipitation_range:";
	fout << recco.precipitation_range.first << "," << recco.precipitation_range.second << "," << endl;
	fout << "soil_moisture_range:";
	fout << recco.soil_moisture_range.first << "," << recco.soil_moisture_range.second << "," << endl;
	fout << "soil_fertility_range:";
	fout << recco.soil_fertility_range.first << "," << recco.soil_fertility_range.second << ",";

	return fout;
}
bool Database::writeToFile(Record data)
{

	
		
	ofstream dataFile;
	dataFile.open("info.txt");
	outfile << data << endl;
	outfile.close();
	
}

ifstream& operator >>(ifstream& fin, Record& recco) 
{


}

Record Database::readFile()
{

	/*
	Database dummy;
	string filename = "info.txt";
	
	ifstream fin;
	fin.open(filename);

	if(fin.is_open()){

		fin >> dummy;

		fin.close();

	}
	else{
		cout << "File cannot be opened" << endl;
	}
	
	cout << dummy << endl;

	*/


}
