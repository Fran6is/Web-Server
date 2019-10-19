#include "WebServer.h"

int main(void)
{
	std::string html{};
	int status = LoadResponse(html, "./SRC/Practice-board.html");

	WebServer test{ "0.0.0.0", 5000,((!status) ? html.c_str(): "<h1>couldn't open file</h1>") };

	status = test.init();

	if (status != 0) return status;

	test.run();

	return 0;
}