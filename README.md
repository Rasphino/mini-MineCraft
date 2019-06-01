## 1. 对fragment shader进行了更改 
主要如下
可以调整maxDist minDist来改变雾的效果.  
float dist = length((cameraPos - cubePos).xyz);//计算方块与camera对距离.  
float maxDist = 50;//雾最远位置  
float minDist = 20;//雾最近位置  
vec3 fogcolor = vec3(0.5, 0.5, 0.5);//雾的颜色  
float fogFactor = (maxDist - dist) / (maxDist - minDist);  
fogFactor = clamp( fogFactor, 0.0, 1.0 );//如果fogFactor <0就取0， >1就取1  
vec3 color = mix(fogcolor, texColor, fogFactor);  
FragColor = vec4(color, 1.0);  
 
## 2. cube.cpp中draw函数增加了cameraPos参数
```cpp
void Cube::Draw(std::string name, glm::vec3 pos, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos)
```
参数使用如下
```cpp
if (name == "soil") {
glBindVertexArray(VAO_soil);
glBindBuffer(GL_ARRAY_BUFFER, VBO_soil);
glEnableVertexAttribArray(2);
glEnableVertexAttribArray(3);
soilShader.use();
soilShader.setMat4("projection", projection);
soilShader.setMat4("view", view);
glUniform1i(glGetUniformLocation(soilShader.ID, "soil"), 0);

glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, pos);
soilShader.setMat4("model", model);
soilShader.setVec3("cubePos", pos);         //新增
soilShader.setVec3("cameraPos", cameraPos); //新增
glDrawArrays(GL_TRIANGLES, 0, 36);
} 
```

