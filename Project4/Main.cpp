#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

const char* load_shader(string file_name) {
	if (file_name == "") return NULL;

	// ソースファイルを開く
	ifstream file(file_name, ios::binary);
	if (!file) {
		cout << "Error: " << file_name << " could not be opened." << endl;
		return NULL;
	}
	file.seekg(0, ios::end);
	int size = file.tellg();
	vector<char> buffer(size);	
	file.seekg(0, ios::beg);
	file.read(buffer.data(), size);
	if (file.fail()) {
		cout << "Error: " << file_name << " could not be read." << endl;
		file.close();
		return NULL;
	}
	file.close();
	char* shader_source = new char[size + 1];
	memcpy(shader_source, buffer.data(), size);
	shader_source[size] = '\0';
	return shader_source;
}

int main(){
	// start GL context and O/S window using the GLAFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	// uncomment these lines if on Apple OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */
	float points[] = {
	 0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	// load shader from file
	const char* vertex_shader = load_shader("test_vs.glsl");

	const char* fragment_shader = load_shader("test_fs.glsl");
	
	

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	delete[] vertex_shader;
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);	
	delete[] fragment_shader;
	GLint err;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &err);

	if (err == GL_FALSE) {
		printf("vertex shader has errors\n");
		GLsizei bufSize;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &bufSize);
		GLsizei length;
		GLchar* infoLog = (GLchar*) malloc(bufSize);
		glGetShaderInfoLog(vs, bufSize, &length, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}

	glGetShaderiv(fs, GL_COMPILE_STATUS, &err);

	if (err == GL_FALSE) {
		printf("fragment shader has errors\n");
	}

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);

	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		// draw points 0 - 3 from the currently boud VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}