#include "app.h"

using namespace lantern;

class empty_app : public app
{
public:
	empty_app(unsigned int const width, unsigned int const height);

protected:
	void update() override;
};

empty_app::empty_app(unsigned int const width, unsigned int const height)
	: app(width, height)
{
}

void empty_app::update()
{
}

int main(int argc, char* argv[])
{
	return empty_app{640, 480}.start();
}
