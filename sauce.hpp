/* 
sauce.hpp
version 0.0.1
https://github.com/rfetree/
Copyright (c) 2018 rfetree <https://github.com/rfetree>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
-----------------------------------------
-----------------------------------------
-----------------------------------------
json.hpp copyright notice
-----------------------------------------

 __|  |   __|     |   | |  JSON for Modern C++
|  |  |__   |  |  | | | |  version 3.2.0
|_____|_____|_____|_|___|  https://github.com/nlohmann/json
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2013-2018 Niels Lohmann <http://nlohmann.me>.
Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <bits/stdc++.h>
#include "json.hpp"
namespace sauce
{
	class sauceResult
	{
		public:
			std::vector<std::string> sourcelinks;
			double similarity;
			void init(std::vector<std::string> ext_urls,std::string d)
			{
				similarity = std::stod(d);
				sourcelinks = ext_urls;
			}
			int number_of_links(void)
			{
				return sourcelinks.size();
			}
	};
	class sauceTagFetch
	{
	private:

		bool replace(std::string& str, std::string& from, std::string& to) {
			size_t start_pos = str.find(from);
			if(start_pos == std::string::npos)
			return false;
			str.replace(start_pos, from.length(), to);
			return true;
		}
		std::string api_key;
		std::string file_path;
		std::string sh_command_format = "curl -F \"file=@FILEPATHHERE\" \"https://saucenao.com/search.php?output_type=2&api_key=APIKEYHERE\" >> \"saucenao.json\"" ;
		std::string sh_command = sh_command_format;
	public:
		void set_params(std::string new_key,std::string new_path)
		{
			api_key = new_key;
			file_path = new_path;
			sh_command = sh_command_format;
			std::string tmp = "APIKEYHERE";
			replace(sh_command,tmp,api_key);
			tmp = "FILEPATHHERE";
			replace(sh_command,tmp,file_path);
		}
		std::vector<sauceResult> get_sauce(void)
		{
			std::system(sh_command.c_str());
			std::ifstream raw_file("saucenao.json");
			nlohmann::json json_obj;
			raw_file >> json_obj;
			auto x = json_obj["results"];
			std::vector<sauceResult> rvec;
			for(auto iter = x.begin();iter!=x.end();++iter)
			{
				sauceResult sr;
				auto curr = *iter;
				std::string similarity = curr["header"]["similarity"];
				auto ext_urls = curr["data"]["ext_urls"];
				std::vector<std::string> v;
				for(auto iter_lv2 = ext_urls.begin();iter_lv2!=ext_urls.end();++iter_lv2)
				{
					//std::cout<<*iter_lv2<<std::endl;
					std::string xstr = "";
					xstr = *iter_lv2;
					v.push_back(xstr);
				}
				//std::cout<<"Similarity: "<<similarity<<std::endl;
				//std::cout<<"Array: "<<ext_urls<<std::endl;
				sr.init(v,similarity);
				rvec.push_back(sr);
			}
			return rvec;
		}
	};
}
