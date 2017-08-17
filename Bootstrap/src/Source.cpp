#include <glm/glm.hpp>
#include <iostream>
using namespace std;
using namespace glm;

int main()
{
	auto v = vec3(1, 1, 1);
	auto v2 = vec3(1, 1, 1);
	v.operator-=(v2);
	
	return 0;
}
