
#include "example_renderer.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <iostream>

void EXAMPLE::Renderer::startup() {

	const std::string vert_src = "#version 430 core\n"
		"layout (location = 0) in vec3 pos;"
		"uniform mat4 model;"
		"void main()"
		"{"
		"   gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);"
		"}";
	const std::string frag_src = "#version 430 core\n"
		"out vec4 frag_color;"
		"void main()"
		"{"
		"   frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";

	f32 vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	u32 vbo, vao, shader, model_uniform;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	if (vbo > 0 && vao > 0) {
		
		m_vbo = vbo;
		m_vao = vao;

		glBindVertexArray(m_vao.value());
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		u32 vert, frag;
		vert = glCreateShader(GL_VERTEX_SHADER);
		frag = glCreateShader(GL_FRAGMENT_SHADER);
		const char* vert_h = vert_src.c_str();
		const char* frag_h = frag_src.c_str();
		glShaderSource(vert, 1, &vert_h, NULL);
		glShaderSource(frag, 1, &frag_h, NULL);
		glCompileShader(vert);
		glCompileShader(frag);

		int  success;
		char infoLog[512];

		glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vert, 512, NULL, infoLog);
			std::cout << "Failed to create vert shader, example rendering will not function\n" << infoLog << std::endl;
			return;
		}

		glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(frag, 512, NULL, infoLog);
			std::cout << "Failed to create frag shader, example rendering will not function\n" << infoLog << std::endl;
			glDeleteShader(vert);
			return;
		}

		u32 shader;
		shader = glCreateProgram();
		glAttachShader(shader, vert);
		glAttachShader(shader, frag);
		glLinkProgram(shader);

		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) { 
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "Failed to link shader program, example rendering will not function\n" << infoLog << std::endl;
		}
		else {
			m_shader = shader;
			m_model_uniform = glGetUniformLocation(m_shader.value(), "model");
		}

		glDeleteShader(vert);
		glDeleteShader(frag);

	}
	else {

		std::cout << "Failed to create vbo and vao, example rendering will not function" << std::endl;

	}

}

void EXAMPLE::Renderer::shutdown() {

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &m_vao.value());
	glDeleteBuffers(1, &m_vbo.value());

}

void EXAMPLE::Renderer::draw(Frame& frame, const glm::vec2& viewport) {
	if (m_vao.has_value()) {

		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(frame.EXAMPLE.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::scale(view, glm::vec3(128.0f, 128.0f, 0.0f));
		glm::mat4 proj = glm::ortho(-(viewport.x / 2), viewport.x / 2, -(viewport.y / 2), viewport.y / 2, -64.0f, 64.0f);
		model = proj * view * model;

		glUseProgram(m_shader.value());
		glUniformMatrix4fv(m_model_uniform.value(), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(m_vao.value());
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo.value());
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}