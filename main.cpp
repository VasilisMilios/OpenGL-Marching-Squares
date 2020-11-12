#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_m.h>

#include <OpenSimplexNoise.h>

#include <vector> 
#include <iostream>
#include <stdlib.h>   

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::vector<float> getOffset(int state);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const int rows = 1000;
const int cols = 1000;
const float size = 2.0 / (rows - 1);

float time = 0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Marching Squares", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    

    Shader gridShader("grid_vertex.glsl", "grid_fragment.glsl");
    Shader gridLineShader("grid_line_vertex.glsl", "grid_line_fragment.glsl");
    Shader lineShader("line_vertex.glsl", "line_fragment.glsl", "line_geometry.glsl");

    OpenSimplexNoise::Noise noise;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float gridLines[(rows + cols) * 4];
    for (int i = 0; i < rows; i++) {
        gridLines[i * 4] = i * size - 1.0f;
        gridLines[i * 4 + 1] = -1.0f;
        gridLines[i * 4 + 2] = i * size - 1.0f;
        gridLines[i * 4 + 3] = 1.0f;
    }

    for (int i = 0; i < cols; i++) {
        gridLines[rows * 4 + i * 4] = -1.0f;
        gridLines[rows * 4 + i * 4 + 1] = i * size - 1.0f;
        gridLines[rows * 4 + i * 4 + 2] = 1.0f;
        gridLines[rows * 4 + i * 4 + 3] = i * size - 1.0f;
    }

    float *gridVertices = (float*) malloc(rows*cols*3*sizeof(float));
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            gridVertices[count++] = i * size - 1.0f;
            gridVertices[count++] = j * size - 1.0f;
            gridVertices[count++] = 0.0f;
        }
    }


    unsigned int gridVBO, gridVAO, gridLineVAO, gridLineVBO, lineVBO, lineVAO;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
   // glBindVertexArray(gridVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
   // glBufferData(GL_ARRAY_BUFFER,sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(gridVertices), gridVertices);
    //glBufferSubData(GL_ARRAY_BUFFER, sizeof(gridVertices), sizeof(gridValues), gridValues);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)(sizeof(gridVertices)));
    //glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &gridLineVAO);
    glGenBuffers(1, &gridLineVBO);
    glBindVertexArray(gridLineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridLineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridLines), gridLines, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, rows * cols * 3 * sizeof(float), gridVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //glBindVertexArray(0);
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    free(gridVertices);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glLineWidth(8);
        glClear(GL_COLOR_BUFFER_BIT);
        float time = glfwGetTime();
        gridLineShader.use();
        glBindVertexArray(gridLineVAO);
        //glDrawArrays(GL_LINES, 0, (rows + cols) * 2);
        gridShader.use();
        lineShader.setFloat("time", time);
        lineShader.setFloat("size", size);
        glBindVertexArray(gridVAO);
        
        //glDrawArrays(GL_POINTS, 0, rows * cols);
        lineShader.use();
        lineShader.setFloat("time", time/10);
        lineShader.setFloat("size", size);
        glBindVertexArray(lineVAO);
        glPointSize(10);
        glDrawArrays(GL_POINTS, 0, rows*cols);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
         // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lineVector.size(), lines, GL_STATIC_DRAW);
        //glDrawArrays(GL_LINES, 0, lineVector.size());
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        time = glfwGetTime();

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}