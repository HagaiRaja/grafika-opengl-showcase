#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#define STB_IMAGE_IMPLEMENTATION

#define WORLD_TOP 30.0
#define WORLD_BOTTOM -1.0
#define WORLD_LEFT -5.0
#define WORLD_RIGHT 15.0
#define WORLD_FRONT -15.0
#define WORLD_BACK 25.0

#define NUMBER_OF_RAIN_PARTICLE 1000
#define PARTICLE_MIN_SPEED 0.1
#define PARTICLE_MAX_SPEED 0.5
#define PARTICLE_SPEED 0.3

#define NUMBER_OF_SMOKE_PARTICLE 200
#define START_X 5
#define START_Y 0.5
#define START_Z 18
#define SMOKE_MAX_LIFETIME 50
#define LIFESPAN_PER_CYCLE 1

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"
#include "learnopengl/camera.h"
#include "learnopengl/filesystem.h"
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

typedef struct{
    float x, y, z;
} point;

typedef struct{
    float x, y, z;
    float speed; // on y axis
} rain;

typedef struct{
    float x, y, z;
    float halftime, decaytime; // on 0-1s move on hor and ver on 1-2s move on ver
    float x_speed, y_speed, z_speed; // on y axis
} smoke;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(5.0f, 5.0f, 20.0f);

//glm::vec3 lightPos(-5.0f, -1.3f, -15.0f);


// ground
glm::vec3 groundPos(5.0f, -1.3f, 5.0f);

int main()
{
    srand (static_cast <unsigned> (time(0)));
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Car Showcase", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("2.2.basic_lighting.vs", "2.2.basic_lighting.fs");
    Shader lampShader("2.2.lamp.vs", "2.2.lamp.fs");
    Shader groundShader("ground.vs", "ground.fs");
    Shader particleShader("particle.vs", "particle.fs");
    Shader waterShader("water.vs", "water.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            -0.5f, -0.5f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -1.0f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  1.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -1.0f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  1.0f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -1.0f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };

    float ground[] = { // consist of two triangle
            10.0f, 10.0f, 10.0f,
            -10.0f, 10.0f, 10.0f,
            -10.0f, -10.0f, 10.0f,

            -10.0f, 10.0f, 10.0f,
            -10.0f, -10.0f, 10.0f,
            10.0f, -10.0f, 10.0f
    };

    // --------------------------------------------------------------------------------------------------
    // READING POSITION DATA
    vector<glm::vec3> scaler;
    vector<glm::vec3> position;
    vector<glm::vec3> color;
    vector<int> objectTexture;

    ifstream imageFile;
    imageFile.open("data.txt", ios::in);
    unsigned int numberOfTriangles = 0;
    int count = 0;
    if (imageFile.is_open())
    {
        int temp_r, temp_g, temp_b, temp_a;
        float R, G, B, A;
        point now;
        do {
            char temp;
            imageFile >> temp;
            if (temp == '#') {
                imageFile >> temp;
                while (temp != '#') {
                    imageFile >> temp;
                }
            }

            imageFile >> now.x >> now.y >> now.z;
            scaler.push_back(glm::vec3(now.x,  now.y,  now.z));
            imageFile >> now.x >> now.y >> now.z;
            position.push_back(glm::vec3(now.x,  now.y,  now.z));

            imageFile >> temp_r >> temp_g >> temp_b;
            R = ((float) temp_r)/255;
            G = ((float) temp_g)/255;
            B = ((float) temp_b)/255;
            color.push_back(glm::vec3(R, G, B));

            int textureInt;
            imageFile >> textureInt;
            objectTexture.push_back(textureInt);

            imageFile >> now.x >> now.y >> now.z;
            count++;
        } while (now.x != 99999 && count != 10000);

        imageFile.close();
    }
    else cout << "Unable to open file";

    if (count == 10000) {
        cout << "Error reading file" << endl;
        return 0;
    }

    // world space positions of our cubes
    glm::vec3 cubePositions[position.size()];

    int iterate = 0;
    for (std::vector<glm::vec3>::const_iterator is = position.begin(); is != position.end(); ++is) {
        cubePositions[iterate] = *is;
        iterate++;
    }

    // world space scale of our cubes
    glm::vec3 cubeScaler[scaler.size()];

    iterate = 0;
    for (std::vector<glm::vec3>::const_iterator is = scaler.begin(); is != scaler.end(); ++is) {
        cubeScaler[iterate] = *is;
        iterate++;
    }

    // world color of our cubes
    glm::vec3 cubeColor[color.size()];

    iterate = 0;
    for (std::vector<glm::vec3>::const_iterator is = color.begin(); is != color.end(); ++is) {
        cubeColor[iterate] = *is;
        iterate++;
    }

    // world color of our cubes
    int cubeTexture[objectTexture.size()];

    iterate = 0;
    for (std::vector<int>::const_iterator is = objectTexture.begin(); is != objectTexture.end(); ++is) {
        cubeTexture[iterate] = *is;
        iterate++;
    }

    cout << position.size() << endl;
    cout << scaler.size() << endl;
    cout << color.size() << endl;

    // --------------------------------------------------------------------------------------------------

    // generating rain
    rain rainParticle[NUMBER_OF_RAIN_PARTICLE];

    for (int i = 0; i < NUMBER_OF_RAIN_PARTICLE; ++i) {
        rainParticle[i].x = static_cast<float>(WORLD_LEFT + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (WORLD_RIGHT - WORLD_LEFT))));
        rainParticle[i].z = static_cast<float>(WORLD_FRONT + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (WORLD_BACK - WORLD_FRONT))));
        rainParticle[i].y = WORLD_TOP;
        rainParticle[i].speed = static_cast<float>(PARTICLE_MIN_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_MAX_SPEED - PARTICLE_MIN_SPEED))));
        cout << rainParticle[i].x << " " << rainParticle[i].y << " " << rainParticle[i].z << " " << rainParticle[i].speed << endl;
    }

    // generating smoke
    smoke smokeParticle[NUMBER_OF_SMOKE_PARTICLE];

    for (int j = 0; j < NUMBER_OF_SMOKE_PARTICLE; ++j) {
        smokeParticle[j].x = START_X;
        smokeParticle[j].y = START_Y;
        smokeParticle[j].z = START_Z;
        smokeParticle[j].decaytime = rand()%(SMOKE_MAX_LIFETIME + 1);
        smokeParticle[j].halftime = smokeParticle[j].decaytime / 2;
        smokeParticle[j].x_speed = static_cast<float>(-PARTICLE_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_SPEED * 2))));
        smokeParticle[j].y_speed = static_cast<float>(PARTICLE_MIN_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_MAX_SPEED - PARTICLE_MIN_SPEED))));
        smokeParticle[j].z_speed = static_cast<float>(PARTICLE_MIN_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_MAX_SPEED - PARTICLE_MIN_SPEED))));
    }

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int groundVAO;
    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2, texture3;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::getPath("textures/tentara.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data_glass = stbi_load(FileSystem::getPath("textures/glass.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data_glass)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_glass);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data_glass);

    // texture 3
    // ---------
    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data_rubber = stbi_load(FileSystem::getPath("textures/rubber.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data_rubber)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_rubber);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data_rubber);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    lightingShader.use();
    lightingShader.setInt("texture1", 0);
    lightingShader.setInt("texture2", 1);
    lightingShader.setInt("texture3", 2);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        particleShader.use();
        particleShader.setVec3("objectColor", 0.0f, 0.0f, 1.0f);
        particleShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        particleShader.setVec3("lightPos", lightPos);
        particleShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        particleShader.setMat4("projection", projection);
        particleShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        particleShader.setMat4("model", model);

        // drawing smoke
        // smoke color
        glm::mat4 colours = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        colours = glm::scale(colours, glm::vec3(0.6f)); // colours are grey
        for (int j = 0; j < NUMBER_OF_SMOKE_PARTICLE; ++j) {
            // update position
            smokeParticle[j].decaytime -= LIFESPAN_PER_CYCLE;
            if (smokeParticle[j].decaytime < 0) {
                smokeParticle[j].x = START_X;
                smokeParticle[j].y = START_Y;
                smokeParticle[j].z = START_Z;
                smokeParticle[j].decaytime = rand()%(SMOKE_MAX_LIFETIME + 1);
                smokeParticle[j].halftime = smokeParticle[j].decaytime / 2;
                smokeParticle[j].x_speed = static_cast<float>(-PARTICLE_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_SPEED * 2))));
                smokeParticle[j].y_speed = static_cast<float>(PARTICLE_MIN_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_MAX_SPEED - PARTICLE_MIN_SPEED))));
                smokeParticle[j].z_speed = static_cast<float>(PARTICLE_MIN_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_MAX_SPEED - PARTICLE_MIN_SPEED))));
            }
            else if (smokeParticle[j].decaytime < smokeParticle[j].halftime) {
                smokeParticle[j].x += smokeParticle[j].x_speed;
                smokeParticle[j].y += smokeParticle[j].y_speed;
                smokeParticle[j].z += smokeParticle[j].z_speed;
            }
            else {
                smokeParticle[j].x += smokeParticle[j].x_speed;
                smokeParticle[j].z += smokeParticle[j].z_speed;
            }

            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, glm::vec3(smokeParticle[j].x, smokeParticle[j].y, smokeParticle[j].z));
            float angle = 0;
            model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            particleShader.setMat4("model", model);

            particleShader.setMat4("aColor", colours);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // be sure to activate shader when setting uniforms/drawing objects
        waterShader.use();
        waterShader.setVec3("objectColor", 0.0f, 0.0f, 1.0f);
        waterShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        waterShader.setVec3("lightPos", lightPos);
        waterShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        waterShader.setMat4("projection", projection);
        waterShader.setMat4("view", view);

        // world transformation
        model = glm::mat4(1.0f);
        waterShader.setMat4("model", model);

        // drawing rain
        // rain color
        colours = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        colours = glm::scale(colours, glm::vec3(0.0f, 1.0f, 1.0f)); // colours are blue light
        for (int i = 0; i < NUMBER_OF_RAIN_PARTICLE; ++i) {
            // update rain
            rainParticle[i].y -= rainParticle[i].speed;
            if (rainParticle[i].y < WORLD_BOTTOM) {
                rainParticle[i].x = static_cast<float>(WORLD_LEFT + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (WORLD_RIGHT - WORLD_LEFT))));
                rainParticle[i].z = static_cast<float>(WORLD_FRONT + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (WORLD_BACK - WORLD_FRONT))));
                rainParticle[i].y = WORLD_TOP;
                rainParticle[i].speed = static_cast<float>(PARTICLE_MIN_SPEED + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX / (PARTICLE_MAX_SPEED - PARTICLE_MIN_SPEED))));
                cout << "regenerate particle " << i << " : " << rainParticle[i].x << " " << rainParticle[i].y << " " << rainParticle[i].z << " " << rainParticle[i].speed << endl;
            }

            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, glm::vec3(rainParticle[i].x, rainParticle[i].y, rainParticle[i].z));
            float angle = 0.0f * i;
            model = glm::scale(model, glm::vec3(0.05f, 0.2f, 0.05f));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            waterShader.setMat4("model", model);

            waterShader.setMat4("aColor", colours);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.0f, 0.0f, 1.0f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        // render boxes
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < scaler.size(); i++)
        {
            if (cubeTexture[i] == 1) {
                // bind textures on corresponding texture units
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture1);
            }
            else if (cubeTexture[i] == 2) {
                // bind textures on corresponding texture units
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture2);
            }
            else if (cubeTexture[i] == 3) {
                // bind textures on corresponding texture units
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture3);
            }
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 0.0f * i;
            model = glm::scale(model, cubeScaler[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            // box color
            glm::mat4 colours = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            colours = glm::scale(colours, cubeColor[i]);
            lightingShader.setMat4("aColor", colours);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);


        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw ground
        groundShader.use();
        groundShader.setMat4("projection", projection);
        groundShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, groundPos);
        model = glm::scale(model, glm::vec3(20.0f, 0.1f, 20.0f)); // a smaller cube
        groundShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

