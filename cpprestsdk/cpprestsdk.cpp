//2020.7.21 百度经验下拷贝
//author 孙宝臻
//目的：C++环境下发布rest服务
//#include <cpprest/http_client.h>
//#include <cpprest/filestream.h>
//
//using namespace utility;                    // Common utilities like string conversions常见的实用程序，如字符串转换
//using namespace web;                        // Common features like URIs.常见功能，如uri。
//using namespace web::http;                  // Common HTTP functionality   通用HTTP功能
//using namespace web::http::client;          // HTTP client features  HTTP客户端功能
//using namespace concurrency::streams;       // Asynchronous streams  异步流
//
//int main(int argc, char* argv[])
//{
//	auto fileStream = std::make_shared<ostream>();
//
//	// Open stream to output file.打开流以输出文件。
//	pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile) {
//		*fileStream = outFile;
//
//		// Create http_client to send the request.  创建http客户端以发送请求。
//		http_client client(U("http://www.bing.com/"));
//
//		// Build request URI and start the request.  构建请求URI并启动请求。
//		uri_builder builder(U("/search"));
//		builder.append_query(U("q"), U("cpprestsdk github"));
//		return client.request(methods::GET, builder.to_string());
//		})
//
//		// Handle response headers arriving.  处理到达的响应标头。
//			.then([=](http_response response) {
//			printf("Received response status code:%u\n", response.status_code());
//
//			// Write response body into the file.  将响应正文写入文件。
//			return response.body().read_to_end(fileStream->streambuf());
//				})
//
//			// Close the file stream. 关闭文件流。
//					.then([=](size_t) {
//					return fileStream->close();
//						});
//
//				// Wait for all the outstanding I/O to complete and handle any exceptions  等待所有未完成的I/O完成并处理任何异常
//				try {
//					requestTask.wait();
//				}
//				catch (const std::exception& e) {
//					printf("Error exception:%s\n", e.what());
//				}
//
//				return 0;
//}