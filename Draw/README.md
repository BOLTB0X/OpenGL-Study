# Draw

![그려 그려](https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EA%B7%B8%EB%A0%A4%EA%B7%B8%EB%A0%A4.gif?raw=true)

## Draw Circle

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

## Sphere

> **위도(latitude)** 와 **경도(longitude)**를 기준으로 좌표를 계산해 삼각형을 연결

<p align="center">
  <table style="width:100%; text-align:center;">
    <tr>
      <td style="text-align:center; vertical-align:middle;">
        <p align="center">
        <img src="https://github.com/BOLTB0X/OpenGL-Study/blob/main/Img/%EA%B5%AC%EA%B7%B8%EB%A6%AC%EA%B8%B0.png?raw=true" alt="Example Image" width="70%"/>
        </p>
      </td>
    </tr>
    <tr>
      <td style="text-align:center; font-size:14px; font-weight:bold;">
      <p align="center">
      Sphere
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

   - stack : 위도 방향 단면

   - sector : 경도 방향 단면

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
    <img src="https://static.thenounproject.com/png/283045-200.png" alt="Example Image" width="70%">
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