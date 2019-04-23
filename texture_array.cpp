#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"
#include "learnopengl/filesystem.h"
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

typedef struct{
    float x, y, z;
} point;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
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
    Shader ourShader("7.1.camera.vs", "7.1.camera.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            -0.5f, -0.5f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f,
            0.5f, -0.5f,  1.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f,  1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f,

            -0.5f,  0.5f,  1.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -1.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  1.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -1.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -1.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  1.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -1.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  1.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  1.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  1.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -1.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -1.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  1.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -1.0f,  0.0f, 1.0f
    };

    // --------------------------------------------------------------------------------------------------
    // READING POSITION DATA
    vector<glm::vec3> scaler;
    vector<glm::vec3> position;
    vector<glm::vec3> color;

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

    cout << position.size() << endl;
    cout << scaler.size() << endl;
    cout << color.size() << endl;

    // --------------------------------------------------------------------------------------------------

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
//    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
//    glEnableVertexAttribArray(2);


    // load and create a texture
    // -------------------------
    unsigned int *texture;
    glGenTextures(3, texture);
    char filenames[5][30] = {
            "textures/tentara.jpg",
            "textures/glass.jpg",
            "textures/rubber.jpg"
    };

    for (int j = 0; j < 3; ++j) {
        glBindTexture(GL_TEXTURE_2D, texture[j]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;

        unsigned char *data = stbi_load(filenames[j], &width, &height, &nrChannels, 0);
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
    }


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture", 0);


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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < scaler.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 0.0f * i;
            model = glm::scale(model, cubeScaler[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            // box color
            glm::mat4 colours = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            colours = glm::scale(colours, cubeColor[i]);
            ourShader.setMat4("aColor", colours);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
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

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}