#include "MathExporter.h"

namespace Engine {

	void MathExporter::exportMath(sol::state& p_lua_state)
	{
		//glm::vec2
		p_lua_state.new_usertype<glm::vec2>("vec2",

			//get constructor
			sol::call_constructor,
			sol::constructors<glm::vec2(const float&, const float&)>(),

			//member
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			"r", &glm::vec2::r,
			"g", &glm::vec2::g,
			
			//metatale of lua
			sol::meta_function::addition, [](const glm::vec2* vec_a, const glm::vec2* vec_b) {return (*vec_a) + (*vec_b); },
			sol::meta_function::subtraction, [](const glm::vec2* vec_a, const  glm::vec2* vec_b) {return (*vec_a) - (*vec_b); },
			sol::meta_function::multiplication, [](const glm::vec2* vec, const float a) {return (*vec) * a; },
			sol::meta_function::division, [](const glm::vec2* vec, const float a) {return (*vec) / a; },
			sol::meta_function::equal_to, [](const glm::vec2* vec_a, const  glm::vec2* vec_b) {return (*vec_a) == (*vec_b); },
			sol::meta_function::to_string, [](const glm::vec2* vec) -> std::string {return glm::to_string(*vec); }
		);

		//glm::vec3
		p_lua_state.new_usertype<glm::vec3>("vec3",

			//get constructor
			sol::call_constructor,
			sol::constructors<glm::vec3(const float&, const float&, const float&)>(),

			//member
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			"r", &glm::vec3::r,
			"g", &glm::vec3::g,
			"b", &glm::vec3::b,

			//metatale of lua
			sol::meta_function::addition, [](const glm::vec3* vec_a, const glm::vec3* vec_b) {return (*vec_a) + (*vec_b); },
			sol::meta_function::subtraction, [](const glm::vec3* vec_a, const  glm::vec3* vec_b) {return (*vec_a) - (*vec_b); },
			sol::meta_function::multiplication, [](const glm::vec3* vec, const float a) {return (*vec) * a; },
			sol::meta_function::division, [](const glm::vec3* vec, const float a) {return (*vec) / a; },
			sol::meta_function::equal_to, [](const glm::vec3* vec_a, const  glm::vec3* vec_b) {return (*vec_a) == (*vec_b); },
			sol::meta_function::to_string, [](const glm::vec3* vec) -> std::string {return glm::to_string(*vec); }
		);

		//glm::vec4
		p_lua_state.new_usertype<glm::vec4>("vec4",

			//get constructor
			sol::call_constructor,
			sol::constructors<glm::vec4(const float&, const float&, const float&, const float&)>(),

			//member
			"x", &glm::vec4::x,
			"y", &glm::vec4::y,
			"z", &glm::vec4::z,
			"w", &glm::vec4::w,
			"r", &glm::vec4::r,
			"g", &glm::vec4::g,
			"b", &glm::vec4::b,
			"a", &glm::vec4::a,

			//metatale of lua
			sol::meta_function::addition, [](const glm::vec4* vec_a, const glm::vec4* vec_b) {return (*vec_a) + (*vec_b); },
			sol::meta_function::subtraction, [](const glm::vec4* vec_a, const  glm::vec4* vec_b) {return (*vec_a) - (*vec_b); },
			sol::meta_function::multiplication, [](const glm::vec4* vec, const float a) {return (*vec) * a; },
			sol::meta_function::division, [](const glm::vec4* vec, const float a) {return (*vec) / a; },
			sol::meta_function::equal_to, [](const glm::vec4* vec_a, const  glm::vec4* vec_b) {return (*vec_a) == (*vec_b); },
			sol::meta_function::to_string, [](const glm::vec4* vec) -> std::string {return glm::to_string(*vec); }
		);

		//glm::mat3
		p_lua_state.new_usertype<glm::mat3>("mat3",

			//get constructor
			sol::call_constructor,
			sol::constructors<
			//get a diagonal matrix
			glm::mat3(const float&),
			//set every number
			glm::mat3(const float&, const float&, const float&, 
					  const float&, const float&, const float&, 
					  const float&, const float&, const float&)>(),

			//metatale of lua
			sol::meta_function::addition, [](const glm::mat3* m_a, const  glm::mat3* m_b) {return (*m_a) + (*m_b); },
			sol::meta_function::subtraction, [](const glm::mat3* m_a, const  glm::mat3* m_b) {return (*m_a) - (*m_b); },
			sol::meta_function::multiplication, [](const glm::mat3* m, const glm::vec4* v) {return (*m) * (*v); },
			sol::meta_function::division, [](const glm::mat3* m, const float a) {return (*m) / a; },
			sol::meta_function::equal_to, [](const glm::mat3* m_a, const  glm::mat3* m_b) {return (*m_a) == (*m_b); },
			sol::meta_function::to_string, [](const glm::mat3* m) {return glm::to_string(*m); }
		);

		//glm::mat4
		p_lua_state.new_usertype<glm::mat4>("mat4",

			//get constructor
			sol::call_constructor,
			sol::constructors<
			//get a diagonal matrix
			glm::mat4(const float&),
			//set every number
			glm::mat4(const float&, const float&, const float&, const float&, 
					  const float&, const float&, const float&, const float&, 
				      const float&, const float&, const float&, const float&, 
				      const float&, const float&, const float&, const float&)>(),

			//metatale of lua
			sol::meta_function::addition, [](const glm::mat4* m_a, const  glm::mat4* m_b) {return (*m_a) + (*m_b); },
			sol::meta_function::subtraction, [](const glm::mat4* m_a, const  glm::mat4* m_b) {return (*m_a) - (*m_b); },
			sol::meta_function::multiplication, [](const glm::mat4* m, const glm::vec4* v) {return (*m) * (*v); },
			sol::meta_function::division, [](const glm::mat4* m, const float a) {return (*m) / a; },
			sol::meta_function::equal_to, [](const glm::mat4* m_a, const  glm::mat4* m_b) {return (*m_a) == (*m_b); },
			sol::meta_function::to_string, [](const glm::mat4* m) {return glm::to_string(*m); }
		);


	}

}