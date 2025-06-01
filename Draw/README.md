# Draw

![그려 그려](https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EA%B7%B8%EB%A0%A4%EA%B7%B8%EB%A0%A4.gif?raw=true)

## Draw Circle

<details>
<summary> 보기/닫기 </summary>

> 중심을 기준으로 삼각형을 여러 개 그려 원을 근사 (삼각 팬 방식)

<p align="center">
  <table style="width:100%; text-align:center;">
    <tr>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EC%9B%90%EA%B7%B8%EB%A6%AC%EA%B8%B0.png?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Circle
      </p>
      </td>
    </tr>
  </table>
</p>

### Ideal

- **삼각형 구성** : 

   중심점 + 원 둘레의 인접한 두 점 = 하나의 삼각형

- **세부 포인트** : 

   - 각도 간격 = `360` / 세그먼트 수

   - 더 많은 세그먼트 -> **더 매끄러운 원**

   ---

### Implement

<details>
<summary> Vertex 구성 </summary>

<p align="center">
    <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0d/Area_of_a_circle.svg/1920px-Area_of_a_circle.svg.png" alt="Example Image" width="70%">
    <br/>
    이미지 출처: ko.wikipedia.org/wiki/원_(기하학)
</p>

```cpp
std::vector<float> circleVertices;

// 중심 (0, 0)
circleVertices.push_back(0.0f); // x
circleVertices.push_back(0.0f); // y

int numSegments = 100;
float radius = 0.5f;

for (int i = 0; i <= numSegments; ++i)
{
	float angle = 2.0f * M_PI * i / numSegments;
	float x = radius * cos(angle);
	float y = radius * sin(angle);

	circleVertices.push_back(x);
	circleVertices.push_back(y);
}
```

---

</details>

</details>




## [Sphere](https://github.com/BOLTB0X/OpenGL-Study/blob/main/Draw/Draw_Sphere/Draw_Sphere/src/Sphere.cpp)

<details>
<summary> 보기/닫기 </summary>

> **위도(latitude)** 와 **경도(longitude)** 를 기준으로 좌표를 계산해 삼각형을 연결

<p align="center">
  <table style="width:100%; text-align:center;">
    <tr>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EA%B5%AC%EA%B7%B8%EB%A6%AC%EA%B8%B0.png?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EA%B5%AC%EA%B7%B8%EB%A6%AC%EA%B8%B02.png?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Sphere
      </p>
      </td>
            <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Phong
      </p>
      </td>
    </tr>
  </table>
</p>

### Ideal

- **Sphere**

   ```cpp
   // θ(세로 각도), φ(가로 각도)
   x = r * sin(θ) * cos(φ)  
   y = r * cos(θ)  
   z = r * sin(θ) * sin(φ)
   ```

   - 적절히 삼각형으로 연결해서 그리면, 구체처럼 보이는 mesh 생성

   - **삼각형 두 개로 사각형을 만들어 구 표면 근사**

   ---

- **삼각형 구성** : 

   - *stack* : 위도 방향 단면

   - *sector* : 경도 방향 단면

   - 각 스택/섹터 사이의 사각형을 두 개의 삼각형으로 분할

   ---

- **세부 포인트** : 

   - 구의 정점들과 인덱스를 생성

   - 노멀은 각 정점 위치를 정규화하여 계산 (구 중심 기준 방향 벡터)

   ---

### Implement

<details>
<summary> 구 생성 </summary>

<p align="center">
    <img src="https://static.thenounproject.com/png/283045-200.png" alt="Example Image" width="50%">
    <br/>
    이미지 출처: thenounproject.com
</p>

1. `createSphere`

    **반지름 radius를 갖는 구(Sphere)** 의 ` vertices` 와 `indices` 를 생성하는 함수


    ```cpp
    class Sphere {
    public:
       Sphere(float radius, int sectorCount, int stackCount);
       ~Sphere();
       // ------------------------------------------------------------------------
       void draw(void) const;
       // ------------------------------------------------------------------------
   private:
       void createSphere(void);
       void normalizeVertices(void);
       void setupMesh(void);
       // ------------------------------------------------------------------------
       float radius;
       int sectorCount, stackCount;
       // ------------------------------------------------------------------------
       std::vector<float> vertices;
       std::vector<unsigned int> indices;
       // ------------------------------------------------------------------------
       unsigned int VAO, VBO, EBO;
       // ------------------------------------------------------------------------
    };
    ```

   ```cpp
   Sphere::Sphere(float radius, int sectorCount, int stackCount)
    : radius(radius), sectorCount(sectorCount), stackCount(stackCount)
   {
       createSphere();
	    normalizeVertices();
       setupMesh();
   }
   ```

   - `radius` : 구의 반지름

   - `sectorCount` : 경도 방향 나눌 개수 (longitude, 동서 방향)

   - `stackCount` : 위도 방향 나눌 개수 (latitude, 남북 방향)

   - `vertices` : Vertex 좌표들

   - `indices` : 삼각형을 구성할 Index

   ---

2. **Vertex 생성** (`vertices.push_back(...)`)

   구 표면상의 (`x` , `y` , `z` ) 좌표를 `stackCount * sectorCount` 만큼 생성

   ```cpp
   for (int i = 0; i <= stackCount; ++i) {
    float stackAngle = PI / 2 - i * PI / stackCount; // 1
    float xy = radius * cosf(stackAngle); // 2
    float z = radius * sinf(stackAngle);  // 3

    // ...
   ```
   
   1. `stackAngle` : 위도 각도 (π/2에서 -π/2까지 내려옴) -> 북극에서 남극까지

   2. `xy` : 해당 위도에서의 수평 원의 반지름

   3. `z` : 해당 스택의 높이 

   ---

   ```cpp
   for (int j = 0; j <= sectorCount; ++j) {
    float sectorAngle = j * 2 * PI / sectorCount; // 4
    // 5
    float x = xy * cosf(sectorAngle);
    float y = xy * sinf(sectorAngle);
    
    // 6
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    // ..
   ```

   4. `sectorAngle` : 경도 (`0` ~ `2π`) -> 원형으로 돌면서 점 찍기

   5. `x` , `y` : 수평 원상의 좌표

   6. `vertices.push_back(...)` : (`x` ,  `y`, `z`)는 구 위의 점 하나

   ---

3. **인덱스 생성** (`indices.push_back(...)`)

   삼각형을 만들기 위해 **두 스택 사이의 4개 정점을 사용해 두 개의 삼각형을 생성**

   ```cpp
   for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);     // 현재 줄 시작 인덱스
        int k2 = k1 + sectorCount + 1;      // 아래 줄 시작 인덱스
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // ...
   ```

   - `k1` , `k2` , `k1 + 1` : 삼각형 1

   - `k1 + 1` , `k2` , `k2 + 1` : 삼각형 2:

   - `(i != 0)` , `(i != stackCount - 1)` : 북극과 남극의 극단점 처리(삼각형 개수 줄이기)

   ---

4. 시각적으로 나타내보면

   ```
   위도 줄1:  ●---●---●---●
              |  /|  /|  /|
              | / | / | / |
              |/  |/  |/  |
   위도 줄2:  ●---●---●---●
   ```

   `k1` 과 `k2 를 활용해 한 쌍의 삼각형을 사각형처럼 생성하고 **구 표면을 근사**

---
</details>

<details>
<summary> 노멀(normal) 벡터 적용 </summary>

<p align="center">
    <img src="https://www.opengl-tutorial.org/assets/images/tuto-13-normal-mapping/TangentVectorFromUVs.png" alt="Example Image" width="70%">
    <br/>
    이미지 출처: www.opengl-tutorial.org
</p>

기존 `vertices` (`x` , `y` , `z`)를 바탕으로, 각 정점의 **노멀 벡터** 를 계산하여 `finalData` 에 (`x` , `y` , `z` , `nx` , `ny` , `nz`) 형식으로 저장

1. **기존 좌표들 저장**

   ```cpp
   std::vector<float> finalData;
   for (size_t i = 0; i < vertices.size(); i += 3) {
        glm::vec3 pos(vertices[i], vertices[i + 1], vertices[i + 2]); // 1
        // 2
		finalData.push_back(pos.x);
		finalData.push_back(pos.y);
		finalData.push_back(pos.z);
   ```

   1. `vertices` 에는 순서대로 `x` , `y` , `z` , `x` , `y` , `z` , ... 가 담겨 있으므로 3개씩 읽어와 `glm::vec3 pos` 에 저장

   2. 원래의 정점 위치를 `finalData` 에 그대로 저장

   ---

2. `normalize`

   ```cpp
      // 3

		glm::vec3 normal = glm::normalize(pos); // 4

        // 5
        finalData.push_back(normal.x);
        finalData.push_back(normal.y);
        finalData.push_back(normal.z);
   }
   ```

   3. 구의 중심이 원점 `(0, 0, 0)` 이고 표면 위 **Vertex** 가 `pos`에 있다면, **Vertex 위치** 자체가 **방향 벡터**

   4. 이 벡터를 정규화(`normalize`) 적용 -> **표면 바깥쪽을 향하는 단위 노멀 벡터**

   5. **노멀 벡터** 를 위치 정보 뒤에 이어서 저장 -> `(x, y, z, nx, ny, nz)` 의 형태로 변환

   ---

</details>

<details>
<summary> Lighting 적용 </summary>

```cpp
// Ambient
float ambientStrength = 0.1;
vec3 ambient = ambientStrength * lightColor;

// Diffuse
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

// Specular
float specularStrength = 0.5;
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;

vec3 result = (ambient + diffuse + specular) * objectColor;
FragColor = vec4(result, 1.0);
```

</details>

</details>

## Toon Shading

<details>
<summary> 보기/닫기 </summary>

<p align="center">
  <table style="width:100%; text-align:center;">
    <tr>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%ED%88%B0%EC%84%B8%EC%9D%B4%EB%94%A9.png?raw=true" alt="Example Image" width="60%"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%ED%88%B0%EC%84%B8%EC%9D%B4%EB%94%A92.png?raw=true" alt="Example Image" width="60%"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%ED%88%B0%EC%84%B8%EC%9D%B4%EB%94%A93.png?raw=true" alt="Example Image" width="60%"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Toon 1
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Toon 2
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Toon 3
      </p>
      </td>
    </tr>
  </table>
</p>

### ideal

1. **개념**

   >  만화나 애니메이션 스타일의 비주얼을 구현하기 위한 셰이딩 기법

   - 일반적인 조명 모델처럼 부드럽게 색이 변화하지 않고, 밝기 단계를 구간별로 나눠서 색상을 뚜렷하게 표현

   - 비연속 조명 : 밝기(`intensity`)를 일정 구간으로 나누어 색을 결정

   - 만화 느낌 : 셀 애니메이션 스타일, 게임·애니메이션에 많이 사용

   ---

2. **세부 포인트** : 

   - 광원 방향과 normal 벡터 사이의 내적(= `intensity`) 계산

   - 밝기를 기준으로 단계 구분 (ex. `if (intensity > 0.8)`...)

   - 해당 구간의 색상으로 출력

   - *윤곽선을 그리기 위해 별도 패스 사용*

   ---

### Implement

<details>
<summary> Toon fragment Shader </summary>

```cpp
float diff = max(dot(norm, lightDir), 0.0);

vec3 intensity = vec3(0.0, 0.0, 0.0);
if (diff > 0.95)
   intensity = vec3(1.0, 0.5, 0.5);
else if (diff > 0.5)
   intensity = vec3(0.6, 0.3, 0.3);
else if (diff > 0.25)
   intensity = vec3(0.4, 0.2, 0.2);
else
   intensity = vec3(0.1, 0.1, 0.1);
```

1. `diff` : 구 의 `norm` 과 `lightDir` 내적 값

2. `if - else if - else` 으로 `intensity` 값을 정함

---

</details>

<details>
<summary> Outline Pass </summary>

2번 그려야함

1. `Sphere` 클래스에 `draw(...)`

   ```cpp
   // Outline 용
   void Sphere::draw(Shader& shader,
				  const glm::mat4& model,
				  const glm::mat4& view,
				  const glm::mat4& projection) const
   {
	   shader.use();
	   shader.setMat4("model", model);
	   shader.setMat4("view", view);
	   shader.setMat4("projection", projection);

	   glBindVertexArray(VAO);
	   glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
   }
   ```

   ```cpp
   void Sphere::draw(Shader& shader,
				const glm::mat4& model,
				const glm::mat4& view,
				const glm::mat4& projection,
				const glm::vec3& lightPos,
				const glm::vec3& viewPos,
				const glm::vec3& lightColor) const
   {
	   shader.use();

	   shader.setMat4("model", model);
	   shader.setMat4("view", view);
	   shader.setMat4("projection", projection);

	   shader.setVec3("lightPos", lightPos);
	   shader.setVec3("viewPos", viewPos);
	   shader.setVec3("lightColor", lightColor);
	   shader.setVec3("objectColor", objectColor);

	   glBindVertexArray(VAO);
	   glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
   }
   ```

   ---

2. `main`

   ```cpp
   //1. 윤곽선
   glEnable(GL_CULL_FACE); // 
   glCullFace(GL_FRONT);  // 
   glEnable(GL_DEPTH_TEST);

	glm::mat4 modelOutline = glm::scale(model, glm::vec3(1.05f));
	sphere.draw(outlineShader, modelOutline, view, projection);

	// 2. 구
	glCullFace(GL_BACK);
	sphere.draw(sphereShader, model, view, projection, lightPos, viewPos, lightColor);
   ```

   1. 윤곽선이 원래 구보다 뒤에 있어야 정상적으로 윤곽선만 남게 해야함

      - `glEnable(GL_CULL_FACE)` , `glCullFace(GL_FRONT)` : 앞쪽면이 아닌 모든 면들은 폐기

      - 살짝 확대한 상태로 렌더링


   2. 그런 다음 구 렌더링

   ---

---

</details>

</details>

## Basic Shading

<details>
<summary> 보기/닫기 </summary>

<p align="center">
  <table style="width:100%; text-align:center;">
    <tr>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%ED%94%8C%EB%9E%AB%EC%84%B8%EC%9D%B4%EB%94%A9.png?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EA%B3%A0%EB%9D%BC%EC%9A%B0%EB%93%9C%EC%84%B8%EC%9D%B4%EB%94%A9.png?raw=true" alt="Example Image" width="60%"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Flat
      </p>
      </td>
            <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Gouraud
      </p>
      </td>
    </tr>
  </table>
</p>

### ideal

1. **개념**

   - **Flat**

      - 삼각형 하나당 하나의 색상을 계산해서, 그 색으로 해당 삼각형 전체에 색을 넣음

      - 면 단위 조명이라고도 함

      - Lighting 연산은 보통 삼각형의 첫 번째 정점(`normal`)을 기준으로 수행

      ---

   - **Gouroud**

      - Vertex에서 Lighting 연산 후, 그 색상을 **보간해서 픽셀에 적용**

      - 노멀 보간이 아니라 Lighting 색상 보간

      - 부드러운 느낌이 있지만 **하이라이트는 흐려짐**

      - Phong 보다 계산량 적지만 디테일은 덜함

      ---

   ---

2. **세부 포인트** :

   - **Flat**

      - vertex shader : `position`만 넘김

      - fragment shader: 조명 계산 안 함

      - `flat` qualifier를 사용해 lighting 값을 Vertex에서만 계산하고 fragment에 전달

      ---

   - **Gouroud**

      - vertex shader : Lighting 연산

      - fragment shader: 색 보정

      ---

   ---

### Implement

<details>
<summary> Flat Shader </summary>

- **flat.vs**

   ```cpp
   #version 330 core
   layout(location = 0) in vec3 aPos;
   layout(location = 1) in vec3 aNormal;

   out flat vec3 Normal;
   out vec3 FragPos;

   uniform mat4 model;
   uniform mat4 view;
   uniform mat4 projection;

   void main(void)
   {
      FragPos = vec3(model * vec4(aPos, 1.0));
      Normal = mat3(transpose(inverse(model))) * aNormal;
      gl_Position = projection * view * vec4(FragPos, 1.0);
   }
   ```

   ---

- **flat.fs**

   ```cpp
   #version 330 core
   in flat vec3 Normal;
   in vec3 FragPos;

   out vec4 FragColor;

   uniform vec3 lightPos;
   uniform vec3 viewPos;
   uniform vec3 lightColor;
   uniform vec3 objectColor;

   void main(void)
   {
	   vec3 norm = normalize(Normal);
	   vec3 lightDir = normalize(lightPos - FragPos);
      float diff = max(dot(norm, lightDir), 0.0);

      vec3 diffuse = diff * lightColor;
      vec3 result = diffuse * objectColor;
      FragColor = vec4(result, 1.0);
   }
   ```

---

</details>

<details>
<summary> Gouraud Shader </summary>

- **gouraud.vs**

   ```cpp
   #version 330 core
   layout(location = 0) in vec3 aPos;
   layout(location = 1) in vec3 aNormal;

   out vec3 VertexColor;

   uniform mat4 model;
   uniform mat4 view;
   uniform mat4 projection;
   uniform vec3 lightPos;
   uniform vec3 lightColor;
   uniform vec3 objectColor;

   void main(void)
   {
	   vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	   vec3 norm = normalize(mat3(transpose(inverse(model))) * aNormal);
	   vec3 lightDir = normalize(lightPos - FragPos);
      float diff = max(dot(norm, lightDir), 0.0);

      vec3 diffuse = diff * lightColor;
      vec3 result = diffuse * objectColor;
      VertexColor = result;

      gl_Position = projection * view * vec4(FragPos, 1.0);
   }
   ```

   ---

- **gouraud.fs**

   ```cpp
   #version 330 core
   in vec3 VertexColor;
   out vec4 FragColor;

   void main(void)
   {
      FragColor = vec4(VertexColor, 1.0);
   }
   ```

   ---

---

</details>

</details>

## [Draw Obj](https://github.com/BOLTB0X/OpenGL-Study/tree/main/Draw/Draw_Obj)

<p align="center">
  <table style="width:100%; text-align:center;">
    <tr>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/Amoghasiddhi.gif?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/Amoghasiddhi-light.gif?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Amoghasiddhi
      </p>
      </td>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Amoghasiddhi Toon
      </p>
      </td>
    </tr>
  </table>
</p>

### ideal

1. **개념**

   - [assimp](https://github.com/assimp/assimp) 를 이용하여 Model 로딩

   - 오브젝트의 구조가 **트리** 구조 이므로 [Mesh](https://github.com/BOLTB0X/OpenGL-Study/blob/main/Draw/Draw_Obj/Draw_Obj/include/mesh.h) , [Model](https://github.com/BOLTB0X/OpenGL-Study/blob/main/Draw/Draw_Obj/Draw_Obj/include/model.h) 클래스 내에서 이에 맞춰 구현

   - 만약 OpenGL 상 좌표 `(0, 0, 0)` 에 있지 않을 경우도 있어 정중앙으로 이동

   - 일반 조명, 툰 세이딩 적용

   ---

2. **세부 포인트** :

   - Model, Mesh 등 [learn OpenGL](https://learnopengl.com/Model-Loading/Model) 오픈 소스 활용

   - `Model` 클래스 내 맴버함수 `GetCenterPosition` 을 활용하여 모델 중앙으로 이동

   - `lightSpot` 을 `(0, 0, 0)` 중심으로 `(x, 0, z)` 으로 회전 시켜 조명 효과 적용

   ---

### Implement

<details>
<summary> Renderer </summary>

`Renderer` 클래스 내에 렌더링 할 객체를 모아둠

```cpp
#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "camera.h"
#include "shader.h"
#include "model.h"
#include "sphere.h"

class Renderer {
public:
    Renderer(Window* window, Camera* camera);
    ~Renderer();
    // -------------------------------------
    void Init();
    void Render(float deltaTime);
    // -------------------------------------
private:
    Window* window;
    Camera* camera;
    // -------------------------------------
    Shader outlineShader;
    Shader amoghasiddhiShader;
    Shader spotShader;
    // -------------------------------------
    Model amoghasiddhi;
    Sphere lightSpot;
    // -------------------------------------
    float elapsedTime = 0.0f;
    float radius = 0.5f;
    float speed = 0.7f;
    // -------------------------------------
};

#endif
```

```cpp
#include "renderer.h"
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 생성자
Renderer::Renderer(Window* window, Camera* camera) :
    window(window), 
    camera(camera),
    outlineShader("glsl/outline.vs", "glsl/outline.fs"),
    amoghasiddhiShader("glsl/buddha.vs", "glsl/toon.fs"),
    spotShader("glsl/lightSpot.vs", "glsl/lightSpot.fs"),
    amoghasiddhi("resources/amoghasiddhi/mia_031182_Amoghasiddhi_64k.obj"),
    lightSpot(1.0f, 20, 20)
{}
// ----------------------------------------------------------------------
Renderer::~Renderer()
{
    outlineShader.~Shader();
    amoghasiddhiShader.~Shader();
    spotShader.~Shader();
}
// ----------------------------------------------------------------------
void Renderer::Init()
{
    stbi_set_flip_vertically_on_load(true);
}
// ----------------------------------------------------------------------
void Renderer::Render(float deltaTime)
{
    elapsedTime += deltaTime;

    // 배경
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)window->width / window->height, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 lightPos = glm::vec3(sin(elapsedTime * speed) * radius, 0.0f, cos(elapsedTime * speed) * radius);
    model = glm::translate(model, -amoghasiddhi.GetCenterPosition());

    // outline
    amoghasiddhi.Render(outlineShader, projection, view, model, 0.005f);
    // main
    amoghasiddhi.Render(amoghasiddhiShader, projection, view, model, camera->Position, lightPos);
    // lightSpot
    lightSpot.Render(spotShader, view, projection, lightPos);
}
// ----------------------------------------------------------------------
```

`main` 에선 `Renderer` 호출

```cpp
// 렌더링 루프
// -------------------------------------------
float lastFrame = 0.0f;
while (!glfwWindowShouldClose(window.GetGLFWWindow())) {
   // 시간 계산
   float currentFrame = static_cast<float>(glfwGetTime());
   float deltaTime = currentFrame - lastFrame;
   lastFrame = currentFrame;
   window.SetDeltaTime(deltaTime);

   // 입력 처리
   window.ProcessInput();

   // 렌더
   renderer.Render(deltaTime);
   
   // 버퍼 교환 및 이벤트 폴링
   glfwSwapBuffers(window.GetGLFWWindow());
   glfwPollEvents();
   // ---------------------------------------------------
}
// -------------------------------------------
```

</details>

<details>
<summary> glsl </summary>

1. 퐁 라이트

   ```cpp
   #version 330 core
   out vec4 FragColor;

   in vec2 TexCoords;
   in vec3 TangentFragPos;
   in vec3 TangentLightPos;
   in vec3 TangentViewPos;
   in mat3 TBN;

   uniform sampler2D texture_diffuse1;
   uniform sampler2D texture_normal1;
   uniform sampler2D texture_specular1;
   uniform sampler2D texture_occlusion1;

   void main()
   {    
      vec3 normal = texture(texture_normal1, TexCoords).rgb;
      normal = normalize(normal * 2.0 - 1.0); 

      // 광원, 뷰 벡터
      vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
      vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

      // 디퓨즈
      float diff = max(dot(lightDir, normal), 0.0);
      vec3 diffuse = diff * texture(texture_diffuse1, TexCoords).rgb;

      // 스페큘러
      vec3 halfwayDir = normalize(lightDir + viewDir);
      float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
      vec3 specular = spec * texture(texture_specular1, TexCoords).rgb;

      // Ambient Occlusion
      float ao = texture(texture_occlusion1, TexCoords).r;

      vec3 ambient = texture(texture_diffuse1, TexCoords).rgb * ao;
      vec3 result = ambient + diffuse + specular;

      FragColor = vec4(result, 1.0);
   }
   ```

   ---

2. 툰

   메인 모델을 로딩 전 외곽선을 표현하기 위해 외곽선 색상으로 먼저 로딩

   ```cpp
   // in Renderer.cpp
   // outline
   amoghasiddhi.Render(outlineShader, projection, view, model, 0.005f);

   // main
   amoghasiddhi.Render(amoghasiddhiShader, projection, view, model, camera->Position, lightPos);
   ```

   ```cpp
   // 모델 외곽선 렌더러
   void Model::Render(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model, float thickness)
   {
      shader.use();
      shader.setMat4("projection", projection);
      shader.setMat4("view", view);
      shader.setMat4("model", model);
      shader.setFloat("thickness", 0.005f);
    
      glCullFace(GL_FRONT);
      draw(shader);
   }
   ```

   그런다음 다시 본 모델 로딩

   ```cpp
   // main 모델 렌더러
   void Model::Render(Shader& shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 position, glm::vec3 lightPos)
   {
      shader.use();
      shader.setMat4("projection", projection);
      shader.setMat4("view", view);
      shader.setMat4("model", model);
      shader.setVec3("viewPos", position);
      shader.setVec3("lightPos", lightPos);

      glCullFace(GL_BACK);
      draw(shader);
   }
   ```

   ```cpp
   #version 330 core
   out vec4 FragColor;

   in vec2 TexCoords;
   in vec3 TangentFragPos;
   in vec3 TangentLightPos;
   in vec3 TangentViewPos;
   in mat3 TBN;

   uniform sampler2D texture_diffuse1;
   uniform sampler2D texture_normal1;
   uniform sampler2D texture_specular1;
   uniform sampler2D texture_occlusion1;

   void main()
   {
      // 노멀 맵 및 정규화
      vec3 normal = texture(texture_normal1, TexCoords).rgb;
      normal = normalize(normal * 2.0 - 1.0); 

      // 광원, 시점 방향
      vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
      vec3 viewDir = normalize(TangentViewPos - TangentFragPos);

      // 툰
      float diff = max(dot(normal, lightDir), 0.0);
      float toonDiffuse = 0.0;

      if (diff > 0.95)
         toonDiffuse = 1.0;
      else if (diff > 0.5)
         toonDiffuse = 0.6;
      else if (diff > 0.25)
         toonDiffuse = 0.3;
      else
         toonDiffuse = 0.1;

      // 툰 하이라이트
      vec3 halfwayDir = normalize(lightDir + viewDir);
      float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
      float toonSpecular = 0.0;

      if (spec > 0.8)
         toonSpecular = 1.0;
      else if (spec > 0.3)
         toonSpecular = 0.4;
      else
         toonSpecular = 0.0;

      //ambient
      vec3 color = texture(texture_diffuse1, TexCoords).rgb;
      float ao = texture(texture_occlusion1, TexCoords).r;
      vec3 ambient = color * ao * 0.2;

      vec3 result = ambient + color * toonDiffuse + vec3(toonSpecular);
      FragColor = vec4(result, 1.0);
   }
   ```

   `toonDiffuse` 방식은 **구** 에서 참고


</details>