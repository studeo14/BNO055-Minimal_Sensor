#include <iostream>
#include "../src/BNO055.h"
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>
#include <asio.hpp>

using asio::ip::tcp;

int main()
{
	BNO055 bno = BNO055(2, BNO055_ADDRESS_A, 1);

	bno.begin(bno.OPERATION_MODE_NDOF);

	usleep(1000000);

	int temp = bno.getTemp();
	std::cout << "Current Temperature: " << temp << " C" << std::endl;

	bno.setExtCrystalUse(true);

	usleep(1000000);
	imu::Quaternion quat = bno.getQuat();
	imu::Vector<3> acc_bframe = bno.getVector(BNO055::VECTOR_ACCELEROMETER);
	usleep(1000000);

	std::cout << "qW: " << quat.w() << " qX: " << quat.x() << " qY: " << quat.y() << " qZ: " << quat.z() << std::endl;

	try{
		asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 9000));

		while(1)
		{
			tcp::socket socket(io_service);
			acceptor.accept(socket);

			std::vector<char> buf(2);
			asio::read(socket, asio::buffer(buf));
			std::string tempString(buf.begin(), buf.end());
			std::cout << tempString << std::endl;
			
			quat = bno.getQuat();
			std::cout << "qW: " << quat.w() << " qX: " << quat.x() << " qY: " << quat.y() << " qZ: " << quat.z() << std::endl;
			std::ostringstream os;
			os << quat.w() << "," << quat.x() << "," << quat.y() << "," << quat.z() << std::endl;

			asio::write(socket, asio::buffer(os.str()));

			usleep(1000);
		}

	} catch(std::exception &e){
		std::cerr << e.what() << std::endl;
	}
    
	return 0;
}
