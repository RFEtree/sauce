#include <bits/stdc++.h>
#include "sauce.hpp"

int main()
{
	std::string my_path = "/path/to/your/image.png";
	std::string my_api_key = "Your API Key";
	sauce::sauceTagFetch st;
	st.set_params(my_path,my_api_key);
	std::vector<sauce::sauceResult> v;
	v = st.get_sauce();
	return 0;
}
