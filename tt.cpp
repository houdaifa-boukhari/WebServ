#include <iostream>
#include <string>
#include <map>
#include <sstream>

// class ParssedRequest {
// public:
//     std::map<std::string, std::string> _headers;
//     std::string _body;
//     bool correct_size;
//     bool chunked;

//     void check_body_size();
// };

// #define WHIET "\033[0m"
// #define GREEN "\033[32m"
// #define BLUE "\033[34m"
// #define RED "\033[31m"
// #define YELLOW "\033[33m"
// #define CYAN "\033[36m"
// #define MAGENTA "\033[35m"


// void ParssedRequest::check_body_size()
// {
//     correct_size = true;
//     chunked = false;
//     std::string::size_type pos = 0;

//     if (_headers.find("Transfer-Encoding") != _headers.end() &&
//         _headers["Transfer-Encoding"] == "chunked")
//     {
//         chunked = true;
//         int i = 0;

//         while (pos < _body.size())
//         {
//             // Find the CRLF after the chunk size
//             std::string::size_type end_line = _body.find("\r\n", pos);
//             if (end_line == std::string::npos)
//             {
//                 std::cout << RED << "end_line not found" << WHIET << std::endl;
//                 correct_size = false;
//                 return;
//             }

//             // Extract and parse chunk size (hex)
//             std::string hex_size = _body.substr(pos, end_line - pos);
//             size_t chunk_size;
//             std::stringstream ss;
//             ss << std::hex << hex_size;
//             if (!(ss >> chunk_size))
//             {
//                 std::cout << RED << "invalid chunk size" << WHIET << std::endl;
//                 correct_size = false;
//                 return;
//             }

//             pos = end_line + 2; // Move past CRLF

//             // Zero-length chunk means end of body
//             if (chunk_size == 0)
//             {
//                 // Expect final CRLF after zero-size chunk
//                 if (_body.substr(pos, 2) != "\r\n")
//                 {
//                     std::cout << RED << "missing final CRLF after last chunk" << WHIET << std::endl;
//                     correct_size = false;
//                 }
//                 return; // Done parsing
//             }

//             // Check if chunk data fits in body
//             if (pos + chunk_size + 2 > _body.size())
//             {
//                 std::cout << RED << "not enough data for chunk" << WHIET << std::endl;
//                 correct_size = false;
//                 return;
//             }

//             // Extract chunk data
//             std::string chunk_data = _body.substr(pos, chunk_size);
//             pos += chunk_size;

//             // Each chunk must end with CRLF
//             if (_body.substr(pos, 2) != "\r\n")
//             {
//                 std::cout << RED << "missing CRLF after chunk data" << WHIET << std::endl;
//                 correct_size = false;
//                 return;
//             }
//             pos += 2;

//             std::cout << "i :" << i++ << ", size: " << chunk_size << std::endl;
//         }

//         // If we exit loop without seeing 0-size chunk, it's invalid
//         std::cout << RED << "no final zero-size chunk found" << WHIET << std::endl;
//         correct_size = false;
//     }

//     std::cout << GREEN << pos << WHIET << std::endl;
// }

// int main()
// {
//     ParssedRequest req;

//     // Set headers
//     req._headers["Transfer-Encoding"] = "chunked";

//     // Example body: correct chunked body (hex sizes + CRLF)
//     req._body = "2\r\nMo\r\n2\r\nMo\r\n0\r\n\r\n";

//     req.check_body_size();

//     std::cout << "Chunked: " << (req.chunked ? "true" : "false") << std::endl;
//     std::cout << "Correct size: " << (req.correct_size ? "true" : "false") << std::endl;

//     // Example of incorrect body (wrong chunk size)
//     // ParssedRequest req2;
//     // req2._headers["Transfer-Encoding"] = "chunked";
//     // req2._body = "5\r\nMo\r\n9\r\nDeveloper\r\n7\r\nNetwork\r\n0\r\n"; // first chunk size wrong

//     // req2.check_body_size();
//     // // std::cout << "\nIncorrect body test:" << std::endl;
//     // std::cout << std::endl;
//     // std::cout << "Chunked: " << (req2.chunked ? "true" : "false") << std::endl;
//     // std::cout << "Correct size: " << (req2.correct_size ? "true" : "false") << std::endl;

//     return 0;
// }
