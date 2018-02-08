void rioreceive::init()
{
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		std::cerr << "Error: Could not create socket" << std::endl;
	} else {
		std::memset((char*)&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(PORT);
		if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
			std::cerr << "Error: could not bind socket" << std::endl;
		}
	}
}

int bytes_to_int(char *bytes, int &a) {
	int ret_value = 0;
	ret_value += (int)(*(bytes+a+0))*16777216;
	ret_value += (int)(*(bytes+a+1))*65536;
	ret_value += (int)(*(bytes+a+2))*256;
	ret_value += (int)(*(bytes+a+3));

	a += 4;

	/*if (ret_value != 0) {
		std::cout << "========\n";
		std::cout << "bytes: " 
			<< (int)(*(bytes+a+0)) << ':' << a << ' '
			<< (int)(*(bytes+a+1)) << ':' << a+1 << ' '
			<< (int)(*(bytes+a+2)) << ':' << a+2 << ' '
			<< (int)(*(bytes+a+3)) << ':' << a+3 << std::endl;
		std::cout << "value: " << ret_value << std::endl;
		std::cout << "========\n";
	}*/
	return ret_value;
}

enum portion
{
	NONE,
	CENTRE_XS,
	ANGLES
};

void rioreceive::get(std::vector<int> &centre_xs, std::vector<double> &angles)
{
	centre_xs.clear();
	angles.clear();
	char buff[1024];
	std::memset(buff, 0, 1024);
	int i = 0;
	recvfrom(sockfd, buff, 1024, 0, (struct sockaddr *)&addr, (unsigned int *)1024);
	/*
	   std::cout << "BUFF: ";
	   for (int i = 0; i < 1023; i++)
	   std::cout << (int)buff[i] << ' ';
	 */
	int data = 0, a = 0;
	portion p = NONE;
	for (int i= 0; i < 1024 && data != 0xE0F; i++) {
		if (data == 0xEC5) {
			p = CENTRE_XS;
		} else if (data == 0xDE6) {
			p = ANGLES;
		} else if (p == CENTRE_XS) {
			centre_xs.push_back(data);
		} else if (p == ANGLES) {
			angles.push_back((double)data/1024-0.436332313);
		}
		data = bytes_to_int(buff, a);
	}
}
