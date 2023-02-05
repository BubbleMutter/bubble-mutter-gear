// std::complex<double> z(2.0, 5.0); // 2 + 5i
std::complex<double> z = 5.0 + 3.0i; // 5.0 + 3i
z.real(100); /* 设置实数部分 */
z.imag(-50); /* 设置虚数部分 */
std::cout << z.real() << std::endl;
std::cout << z.imag() << std::endl;
