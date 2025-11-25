/*
Proyecto Final para la materia de Computación Gráfica e Interación Humano Computadora Teoría
Profesor: Roman Balbuena Carlos Aldair
Autor del proyecto: Enrique Medrano Solano
No.Cuenta: 320152841
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

// ---------------Declaración de variables-----------------------

//Animación del pescado
glm::vec3 centroPecera = glm::vec3(-61.0f, 2.5f, -5.5f);
float radioNadar = 1.12f;
float velocidadNadar = 0.8f;
float anguloNado = 0.0f;

//ANimación Silla
bool animacionSillaActiva = false;
glm::vec3 posSilla = glm::vec3(-50.0f, -1.5f, -5.0f);
float anguloGiroSilla = 0.0f; 
float anguloCaidaSilla = 0.0f; 
bool sillaChoco = false;   //Bandera para la pared y silla
float velocidadSilla = 0.3f;  
float limitePared = -57.0f;

//Puertas
float rotation = 0.0f;
float rotationCuarto = 0.0f;
float movOffset = 5.0f;
//Corrediza
float deslizamiento = -40.0f;
float deslizamientoOffset = 0.15f;
float maxDeslizamiento = 2.5f;

//Pantalla
float offsetPantallaCambia = 0.0f;
float offsetPantalla = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

// ------ Textures -------------
Texture pisoTexture;

//Casa
Texture pisoCasaTexture;
Texture VidrioCasatexture;
Texture Chimeneatexture;
Texture ParedExteriortexture;
Texture TechoTexture;
Texture TubosTexture;
Texture marcoVentana;
Texture VayaTexture;
Texture ArbolTexture;
Texture PuertaPrincipalTexture;
Texture PuertaCorredizaTexture;
Texture PuertaCuartoTexture;

//Living Room
Texture TelevisionTexture;
Texture botonesTeleTexture;
Texture sofaTexture;
Texture mesaTexture;
Texture mesaPataSofa;
Texture PantallaTexture;
Texture MacetaTexture;
Texture TierraTexture;
Texture TalloTexture;
Texture HojaTexture;
Texture Bowltexture;
Texture PalomitasTexture;

//Cuarto de Ferb y Phineas
//Cama Ferb
Texture baseCamaTexture;
Texture SabanaTexture;
Texture AlmohadaTexture;
Texture Colchontexture;
Texture Caberceratexture;

//Moai
Texture MoaiTexture;

//Pecera
Texture Rock1Texture;
Texture Rock2Texture;
Texture Rock3Texture;
Texture Rock4Texture;
Texture Rock5Texture;
Texture Rock6Texture;
Texture Rock7Texture;
Texture AcuarioTexture;
Texture PiedraTexture;

//Pez
Texture Pescadotexture;

//Cabina telefonica
Texture BaseCabinaTexture;
Texture AmarilloCabinaTexture;
Texture TornillosTexture;
Texture DetallesTelefonoTexture;


//--- Models ------

// -----NPCS---------
Model Baljeet;

Model PerryTorso;
Model PerryMI;	//mano izq
Model PerryMD;	//mano der
Model PerryPI;	//pie izq
Model PerryPD;	//pie der
Model PerryS;	//sombrero
Model PerryC;	//Cola

Model BuffordTorso;
Model BuffordMI;	//Mano izq
Model BuffordMD;	//Mano der
Model BuffordPI;	//Pie izq
Model BuffordPD;	//Pie der
Model BuffordCabeza;

// ----- Objetos ------------

//Casa Phineas and Ferb
Model CasaPF_M;
Model PuertaPinci_M;
Model CorredizaDes_M;
Model Corrediza_M;
Model PuertaCuarto_M;

//Living Room
Model TelevisionModel;
Model mesa_M;
Model sofa_M;
Model Planta_M;
Model Palomitas_M;

//Cuarto de Ferb y Phineas
Model CamaFerb_M;
Model Moai_M;
Model Pecera_M;
Model Pescado_M;
Model CabinaTelefono_M;
Model Silla_M;

//Skyboxes
Skybox skybox;

//Arbol
Model EntradaArbolS;
Model CopaArbolS;
Model ArbolS;


//materiales
Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	unsigned int pantallaIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat pantallaVertices[] = {
		// x      y      z        u     v         nx    ny    nz
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	Mesh* pantallaMesh = new Mesh();
	pantallaMesh->CreateMesh(pantallaVertices, pantallaIndices, 32, 6);
	meshList.push_back(pantallaMesh);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	glEnable(GL_DEPTH_TEST);

	CreateObjects();
	CreateShaders();

	//Posición del ovni para que se calcule cada frame
	glm::vec3 ovniPos = glm::vec3(-80.0f, 50.0f, -25.0f);

	// Cámara
	camera = Camera(glm::vec3(-150.0f, 0.0f, -150.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	//--------------Carga de Texturas----------------------------------------

	// Textura del pasto
	pisoTexture = Texture("Textures/Pasto.jpg");
	pisoTexture.LoadTextureA();

	//Texturas Casa
	pisoCasaTexture = Texture("Textures/Piso.jpg");
	pisoCasaTexture.LoadTextureA();
	VidrioCasatexture = Texture("Textures/Vidrio.jpg");
	VidrioCasatexture.LoadTextureA();
	Chimeneatexture = Texture("Textures/Chime.png");
	Chimeneatexture.LoadTextureA();
	ParedExteriortexture = Texture("Textures/ParedExterior.jpg");
	ParedExteriortexture.LoadTextureA();
	TechoTexture = Texture("Textures/Techo.jpg");
	TechoTexture.LoadTextureA();
	TubosTexture = Texture("Textures/TubosTexture.jpg");
	TubosTexture.LoadTextureA();
	marcoVentana = Texture("Textures/MarcoVentana.jpg");
	marcoVentana.LoadTextureA();
	VayaTexture = Texture("Textures/vaya.jpg");
	VayaTexture.LoadTextureA();
	PuertaPrincipalTexture = Texture("Textures/Puerta.png");
	PuertaPrincipalTexture.LoadTextureA();
	PuertaCorredizaTexture = Texture("Textures/PuertaCorrediza.png");
	PuertaCorredizaTexture.LoadTextureA();
	PuertaCuartoTexture = Texture("Textures/Cuarto.png");
	PuertaCuartoTexture.LoadTextureA();

	//Arbol
	ArbolTexture = Texture("Textures/_6_tree.png");
	ArbolTexture.LoadTextureA();

	//Living Room
	//Televisión
	TelevisionTexture = Texture("Textures/tv.png");
	TelevisionTexture.LoadTextureA();
	botonesTeleTexture = Texture("Textures/botones.png");
	botonesTeleTexture.LoadTextureA();
	PantallaTexture = Texture("Textures/pantall.jpg");
	PantallaTexture.LoadTextureA();
	//Sofa
	sofaTexture = Texture("Textures/sofa.jpg");
	sofaTexture.LoadTextureA();
	//Mesa
	mesaTexture = Texture("Textures/mesa.png");
	mesaTexture.LoadTextureA();
	mesaPataSofa = Texture("Textures/PatasMesa.png");
	mesaPataSofa.LoadTextureA();
	//Planta
	TierraTexture = Texture("Textures/tierra.jpg");
	TierraTexture.LoadTextureA();
	MacetaTexture = Texture("textures/maceta.jpg");
	MacetaTexture.LoadTextureA();
	TalloTexture = Texture("Textures/tallo.jpg");
	TalloTexture.LoadTextureA();
	HojaTexture = Texture("Textures/Hojas.jpg");
	HojaTexture.LoadTextureA();
	//Bowl palomitas
	Bowltexture = Texture("Textures/bowl.jpg");
	Bowltexture.LoadTextureA();
	PalomitasTexture = Texture("Textures/palomitas.jpg");
	PalomitasTexture.LoadTextureA();

	//Cuarto de Phienas and Ferb
	//CamaFerb
	baseCamaTexture = Texture("Textures/baseCama.jpg");
	baseCamaTexture.LoadTextureA();
	SabanaTexture = Texture("Textures/sabana.jpg");
	SabanaTexture.LoadTextureA();
	Colchontexture = Texture("Textures/colchon.jpg");
	Colchontexture.LoadTextureA();
	AlmohadaTexture = Texture("Textures/almohada.jpg");
	AlmohadaTexture.LoadTextureA();
	Caberceratexture = Texture("Textures/cabecera.jpg");
	Caberceratexture.LoadTextureA();

	//Pecera
	Rock1Texture = Texture("Textures/12987_saltwater_aquarium_rock_1_diff.jpg");
	Rock1Texture.LoadTextureA();
	Rock2Texture = Texture("Textures/12987_saltwater_aquarium_rock_2_diff.jpg");
	Rock2Texture.LoadTextureA();
	Rock3Texture = Texture("Textures/12987_saltwater_aquarium_rock_3_diff.jpg");
	Rock3Texture.LoadTextureA();
	Rock4Texture = Texture("Textures/12987_saltwater_aquarium_rock_4_diff.jpg");
	Rock4Texture.LoadTextureA();
	Rock5Texture = Texture("Textures/12987_saltwater_aquarium_rock_5_diff.jpg");
	Rock5Texture.LoadTextureA();
	Rock6Texture = Texture("Textures/12987_saltwater_aquarium_rock_6_diff.jpg");
	Rock6Texture.LoadTextureA();
	Rock7Texture = Texture("Textures/12987_saltwater_aquarium_rock_7_diff.jpg");
	Rock7Texture.LoadTextureA();
	// Arena del acuario
	AcuarioTexture = Texture("Textures/12987_saltwater_aquarium_sand_diff.jpg");
	AcuarioTexture.LoadTextureA();
	// Piedra general
	PiedraTexture = Texture("Textures/12987_saltwater_aquarium_stone_diff.jpg");
	PiedraTexture.LoadTextureA();
	//Pez
	Pescadotexture = Texture("Textures/fish.jpg");
	Pescadotexture.LoadTextureA();

	//Cabina Telefonica
	BaseCabinaTexture = Texture("Textures/Rojo.jpg");
	BaseCabinaTexture.LoadTextureA();
	TornillosTexture = Texture("Textures/metal.jpg");
	TornillosTexture.LoadTextureA();



	//--------------Carga de Modelos----------------------------------------

	// ----------------- NPCS -----------------
	Baljeet = Model();
	Baljeet.LoadModel("Models/Baljeet.obj");

	PerryTorso = Model();
	PerryTorso.LoadModel("Models/NPCS/Perry/PerryTorso.obj");
	PerryMI = Model();
	PerryMI.LoadModel("Models/NPCS/Perry/PerryManoIzq.Obj");
	PerryMD = Model();
	PerryMD.LoadModel("Models/NPCS/Perry/PerryManoDer.obj");
	PerryPI = Model();
	PerryPI.LoadModel("Models/NPCS/Perry/PerryPieIzq.obj");
	PerryPD = Model();
	PerryPD.LoadModel("Models/NPCS/Perry/PerryPieDer.obj");
	PerryS = Model();
	PerryS.LoadModel("Models/NPCS/Perry/PerrySombrero.obj");
	PerryC = Model();
	PerryC.LoadModel("Models/NPCS/Perry/PerryCola.obj");

	BuffordTorso = Model();
	BuffordTorso.LoadModel("Models/NPCS/Bufford/TorsoBufford.obj");
	BuffordMI = Model();
	BuffordMI.LoadModel("Models/NPCS/Bufford/BrazoIzqBufford.obj");
	BuffordMD = Model();
	BuffordMD.LoadModel("Models/NPCS/Bufford/BrazoDerBufford.obj");
	BuffordPI = Model();
	BuffordPI.LoadModel("Models/NPCS/Bufford/PieIzqBufford.obj");
	BuffordPD = Model();
	BuffordPD.LoadModel("Models/NPCS/Bufford/PieDerBufford.obj");
	BuffordCabeza = Model();
	BuffordCabeza.LoadModel("Models/NPCS/Bufford/CabezaBufford.obj");


	// ---------------- Entorno -----------------------

	//Casa Phineas and ferb
	CasaPF_M = Model();
	CasaPF_M.LoadModel("Models/CasaFB.obj");
	PuertaPinci_M = Model();
	PuertaPinci_M.LoadModel("Models/PuertaPrincipal.obj");
	CorredizaDes_M = Model();
	CorredizaDes_M.LoadModel("Models/CorredizaDes.obj");
	Corrediza_M = Model();
	Corrediza_M.LoadModel("Models/Corrediza.obj");
	PuertaCuarto_M = Model();
	PuertaCuarto_M.LoadModel("Models/PuertaCuarto.obj");

	//Arbol
	ArbolS = Model();
	ArbolS.LoadModel("Models/Arbolito.obj");

	//Living Room
	//Televisión
	TelevisionModel = Model();
	TelevisionModel.LoadModel("Models/Television.obj");
	//Sofa
	sofa_M = Model();
	sofa_M.LoadModel("Models/sofa.obj");
	//Mesa
	mesa_M = Model();
	mesa_M.LoadModel("Models/mesa.obj");
	//Planta
	Planta_M = Model();
	Planta_M.LoadModel("Models/planta.obj");
	//Bowl palomitas
	Palomitas_M = Model();
	Palomitas_M.LoadModel("Models/palomitas.obj");

	//Cuarto de Phienas and Ferb
	//CamaFerb
	CamaFerb_M = Model();
	CamaFerb_M.LoadModel("Models/CamaFerb.obj");
	//Moai
	Moai_M = Model();
	Moai_M.LoadModel("Models/Moai.obj");
	//Pecera
	Pecera_M = Model();
	Pecera_M.LoadModel("Models/Pecera.obj");
	//Pez
	Pescado_M = Model();
	Pescado_M.LoadModel("Models/12265_Fish_v1_L2.obj");
	//Cabina telefonica
	CabinaTelefono_M = Model();
	CabinaTelefono_M.LoadModel("Models/CabinaTel.obj");

	//Silla
	Silla_M = Model();
	Silla_M.LoadModel("Models/Silla.obj");

	// ----------------------- SKYBOX -------------------------------

	//SKYBOX DÍA
	std::vector<std::string> skyboxFacesDia;
	skyboxFacesDia.push_back("Textures/Skybox/Dia/nx.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/px.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/ny.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/py.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/nz.jpg");
	skyboxFacesDia.push_back("Textures/Skybox/Dia/pz.jpg");
	skybox = Skybox(skyboxFacesDia);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// Luz direccional principal (estática)
	mainLight = DirectionalLight(
		1.0, 1.0, 1.0,
		0.3, 0.4,
		0.3, -1.0, 0.5
	);

	// Dejamos los contadores en 0
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	//Corrediza
	deslizamiento = -100.0f;


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Animación que se activa al presionar la tecla J, permite abrir o cerrar la Puerta Principal.
		if (mainWindow.getAbrirCerrarPuerta())
		{
			if (rotation > 0.0f)
			{
				rotation -= movOffset * deltaTime;
			}
		}
		else
		{
			if (rotation < 90.0f)
			{
				rotation += movOffset * deltaTime;
			}
		}

		// Animación que se activa al presionar la tecla G, permite abrir o cerrar la Puerta Cuarto.
		if (mainWindow.getAbrirCerrarCuarto())
		{
			if (rotationCuarto > 0.0f)
			{
				rotationCuarto -= movOffset * deltaTime;
			}
		}
		else
		{
			if (rotationCuarto < 90.0f)
			{
				rotationCuarto += movOffset * deltaTime;
			}
		}

		//Aminación para puerta corrediza, se activa presionando la tecla H.
		if (mainWindow.getAbrirCerrarCorrediza())
		{
			if (deslizamiento < maxDeslizamiento)
			{
				deslizamiento += deslizamientoOffset * deltaTime;
			}
		}
		else
		{
			if (deslizamiento > 0.0f)
			{
				deslizamiento -= deslizamientoOffset * deltaTime;
			}
		}

		//Logica y animación de la silla
		if (mainWindow.getsKeys()[GLFW_KEY_F])
		{
			animacionSillaActiva = true; // Activamos el "switch"
		}

		if (animacionSillaActiva)
		{
			if (!sillaChoco)
			{
				posSilla.x -= velocidadSilla * deltaTime;
				anguloGiroSilla += 300.0f * deltaTime;
				if (posSilla.x <= limitePared)
				{
					posSilla.x = limitePared;
					sillaChoco = true;
				}
			}
			else
			{
				if (anguloCaidaSilla < 90.0f)
				{
					anguloCaidaSilla += 150.0f * deltaTime;
				}
			}
		}

		//Mover al pez
		float anguloPez = now * velocidadNadar;
		glm::vec3 pezPos;
		pezPos.x = centroPecera.x + sin(now * velocidadNadar) * radioNadar;
		pezPos.z = centroPecera.z + cos(now * velocidadNadar) * radioNadar;
		pezPos.y = centroPecera.y + sin(now * 4.0f) * 0.1f;
		anguloNado = (now * velocidadNadar) * toRadians * 75.0f;
		glfwPollEvents();

		// Control de cámara estándar
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Lógica de Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		// Configuración de cámara estándar
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		// Enviamos 0 luces puntuales y 0 spotlights
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		// Renderizar el Piso (Pasto)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();


		// --- INICIO DE OBJETOS ---

		//Árbol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, -2.0f, 85.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArbolS.RenderModel();


		//Casa Phineas and Ferb
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f)); // Usa la posición animada
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		VidrioCasatexture.UseTexture();
		ParedExteriortexture.UseTexture();
		pisoCasaTexture.UseTexture();
		Chimeneatexture.UseTexture();
		TechoTexture.UseTexture();
		TubosTexture.UseTexture();
		marcoVentana.UseTexture();
		VayaTexture.UseTexture();
		CasaPF_M.RenderModel();

		//Puerta Principal
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, rotation * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Abre y Cierra con J
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaPrincipalTexture.UseTexture();
		PuertaPinci_M.RenderModel();

		//Puerta Corrediza
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 9.0f, 34.8f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaCorredizaTexture.UseTexture();
		Corrediza_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 9.0f, 34.8f));
		model = glm::translate(model, glm::vec3(deslizamiento, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaCorredizaTexture.UseTexture();
		CorredizaDes_M.RenderModel();

		//Puerta cuarto
		model = modelaux;
		model = glm::translate(model, glm::vec3(-27.0f, 0.0f, 7.5f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, rotationCuarto * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //Abre y Cierra con J
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaCuartoTexture.UseTexture();
		PuertaCuarto_M.RenderModel();


		//-----------------------------------Personajes---------------------------------

		//  Baljeet
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.0f, -2.0f, 80.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Baljeet.RenderModel();

		// PERRY EL ORNITORRINCO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.5f, 0.0f, 32.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PerryTorso.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PerryPI.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PerryPD.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PerryMI.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PerryMD.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PerryS.RenderModel();
		model = modelaux;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PerryC.RenderModel();

		// BUFFORD
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 85.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -270 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BuffordTorso.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BuffordPI.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BuffordPD.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BuffordMI.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BuffordMD.RenderModel();
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BuffordCabeza.RenderModel();


		//Living Room
		//Sofa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.0f, -1.5f, 29.6f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sofa_M.RenderModel();
		//Mesa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -1.5f, 29.8f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa_M.RenderModel();

		//Planta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-18.0f, -1.5f, 31.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Planta_M.RenderModel();

		//Bowl palomitas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.5f, 0.0f, 32.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Palomitas_M.RenderModel();

		//TV y logica para la animación de la pantalla.
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.8f, 24.8f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TelevisionModel.RenderModel();

		//Animación
		offsetPantallaCambia += 0.0005;
		if (offsetPantallaCambia > 1.0)
			offsetPantallaCambia = 0.0;
		offsetPantalla = 0.0;
		toffset = glm::vec2(offsetPantallaCambia, offsetPantalla);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.84f, -1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.1f, 1.0f, 2.22f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PantallaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f, 0.0f)));

		//Cuarto de Phienas and Ferb
		//CamaFerb
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-47.0f, -1.5f, -2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CamaFerb_M.RenderModel();

		//Moai
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.0f, -1.5f, -2.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Moai_M.RenderModel();

		//Pecera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-61.0f, 1.0f, -5.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pecera_M.RenderModel();

		//pescado
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-61.0f, 2.5f, -5.5f));
		model = glm::translate(model, pezPos);
		model = glm::rotate(model, -anguloNado, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pescado_M.RenderModel();

		//Cabina Telefonica
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.5f, -1.5f, 6.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CabinaTelefono_M.RenderModel();

		//Silla
		model = glm::mat4(1.0);
		model = glm::translate(model, posSilla);
		if (sillaChoco)
		{
			model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, anguloCaidaSilla * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		}
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Silla_M.RenderModel();

		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}