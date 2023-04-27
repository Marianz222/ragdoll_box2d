#include "Simulacion.h"

//Constructor de la clase
Simulacion::Simulacion() {

	//Establece las variables para la ventana
	ventana_x = 800;
	ventana_y = 600;
	nombre_programa = "Ragdoll - Box2d";

	//Establece la gravedad
	gravedad = {0.0f, 0.01f};

	//Crea la ventana
	ventana = new RenderWindow(VideoMode(ventana_x, ventana_y), nombre_programa);

	cout << "[INFO]: Ventana creada" << endl;

	//Crea la cámara y la asigna como vista a la ventana
	View* camara;
	Vector2f dimensiones_camara = {float(ventana_x / 1.1), float(ventana_y / 1.1)};
	Vector2f centro_camara = {float(ventana_x / 2), float(ventana_y / 2)};
	camara = new View(centro_camara, dimensiones_camara);

	ventana->setView(*camara);

	cout << "[INFO]: Camara creada" << endl;

	//DIMENSIONES: 1 metro = 50 pixeles

	//Fija las dimensiones en metros
	dimensiones_cabeza_m = {0.25f, 0.25f};
	dimensiones_cuerpo_m = {0.3f, 0.4f};
	dimensiones_extremidad_m = {0.15f, 0.5f};
	dimensiones_suelo_m = {12.0f, 0.4f};

	//Fija las dimensiones en pixeles
	dimensiones_cabeza_p = {dimensiones_cabeza_m.x * PIXELES_POR_METRO, dimensiones_cabeza_m.y * PIXELES_POR_METRO };
	dimensiones_cuerpo_p = {dimensiones_cuerpo_m.x * PIXELES_POR_METRO, dimensiones_cuerpo_m.y * PIXELES_POR_METRO };
	dimensiones_extremidad_p = { dimensiones_extremidad_m.x * PIXELES_POR_METRO, dimensiones_extremidad_m.y * PIXELES_POR_METRO };
	dimensiones_suelo_p = { dimensiones_suelo_m.x * PIXELES_POR_METRO, dimensiones_suelo_m.y * PIXELES_POR_METRO };

	//Crea el controlador de eventos
	gestor_eventos = new Event;
	cout << "[INFO]: Gestor de eventos creado" << endl;

	//Llama la función para crear los visuales
	crearVisuales();
	cout << "[INFO]: Creacion de visuales completada" << endl;

	//Llama la función para crear los objetos físicos
	crearFisicas();
	cout << "[INFO]: Creacion de objetos fisicos completada" << endl;

	fuerzas_movimiento[0] = {0.0f, -10.0f};
	fuerzas_movimiento[1] = { 10.0f, 0.0f };
	fuerzas_movimiento[2] = { -10.0f, 0.0f };

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Método que se encarga de crear la parte visual con SFML.
void Simulacion::crearVisuales() {

	//Referencias del arreglo:
	// 1 = Cabeza
	// 2 = Cuerpo
	// 3 = Brazo Der.
	// 4 = Brazo Izq.
	// 5 = Pierna Der.
	// 6 = Pierna Izq.

	//Se construye la cabeza
	visuales_ragdoll[0].setSize(dimensiones_cabeza_p);
	visuales_ragdoll[0].setFillColor(Color::White);
	visuales_ragdoll[0].setOrigin(dimensiones_cabeza_p.x / 2, dimensiones_cabeza_p.y / 2);

	//Se construye el cuerpo
	visuales_ragdoll[1].setSize(dimensiones_cuerpo_p);
	visuales_ragdoll[1].setFillColor(Color::Cyan);
	visuales_ragdoll[1].setOrigin(dimensiones_cuerpo_p.x / 2, dimensiones_cuerpo_p.y / 2);

	//Se construyen los brazos
	for (int i = 2; i < 4; i++) {

		visuales_ragdoll[i].setSize(dimensiones_extremidad_p);
		visuales_ragdoll[i].setFillColor(Color::Blue);
		visuales_ragdoll[i].setOrigin(dimensiones_extremidad_p.x / 2, dimensiones_extremidad_p.y / 2);

	}

	//Se construye la pierna derecha
	visuales_ragdoll[4].setSize(dimensiones_extremidad_p);
	visuales_ragdoll[4].setFillColor(Color::Magenta);
	visuales_ragdoll[4].setOrigin(dimensiones_extremidad_p.x / 2, dimensiones_extremidad_p.y / 2);

	//Se construye la pierna izquierda
	visuales_ragdoll[5].setSize(dimensiones_extremidad_p);
	visuales_ragdoll[5].setFillColor(Color::Red);
	visuales_ragdoll[5].setOrigin(dimensiones_extremidad_p.x / 2, dimensiones_extremidad_p.y / 2);

	//Se construye el suelo
	visuales_suelo.setSize(dimensiones_suelo_p);
	visuales_suelo.setFillColor(Color::Green);
	visuales_suelo.setOrigin(dimensiones_suelo_p.x / 2, dimensiones_suelo_p.y / 2);

	//Se construye el techo
	visuales_techo.setSize(dimensiones_suelo_p);
	visuales_techo.setFillColor(Color::Green);
	visuales_techo.setOrigin(dimensiones_suelo_p.x / 2, dimensiones_suelo_p.y / 2);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Método encargado de crear la parte física con Box2d.
void Simulacion::crearFisicas() {

	//Creación del mundo usando la gravedad previamente establecida
	mundo = new b2World(gravedad);

	///////////////////////////////////

	//FIGURAS

	//Crea las figuras que se usarán sobre los cuerpos
	figuras[0].SetAsBox(dimensiones_cabeza_m.x, dimensiones_cabeza_m.y);
	figuras[1].SetAsBox(dimensiones_cuerpo_m.x, dimensiones_cuerpo_m.y);
	figuras[2].SetAsBox(dimensiones_extremidad_m.x, dimensiones_extremidad_m.y);
	figuras[3].SetAsBox(dimensiones_suelo_m.x, dimensiones_suelo_m.y);

	crearCuerpos();

	crearFijadores();

	//crearArticulaciones();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Crea los cuerpos del proyecto.
void Simulacion::crearCuerpos() {

	//Posiciona cada extremidad donde corresponde
	b2Vec2 posicion_cabeza = { 4.0f, 1.68f };
	b2Vec2 posicion_cuerpo = { 4.0f, 2.0f };
	b2Vec2 posicion_brazo_der = { 4.23f, 2.05f };
	b2Vec2 posicion_brazo_izq = { 3.75f, 2.05f };
	b2Vec2 posicion_pierna_der = { 4.07f, 2.45f };
	b2Vec2 posicion_pierna_izq = { 3.92f, 2.45f };

	b2Vec2 posicion_suelo = { 2.0f, 5.8f };
	b2Vec2 posicion_techo = { 2.0f, 3.0f };

	//Rota los cuerpos
	float rotacion_brazo_der = 45.0f * (b2_pi / 180.0f);
	float rotacion_brazo_izq = -45.0f * (b2_pi / 180.0f);
	float rotacion_pierna_der = 25.0f * (b2_pi / 180.0f);
	float rotacion_pierna_izq = -25.0f * (b2_pi / 180.0f);

	//DEFINICIÓN DE CUERPOS

	//OBJETOS

	//Propiedades para la definición del suelo
	definicion_suelo.type = b2_staticBody;
	definicion_suelo.allowSleep = false;
	definicion_suelo.position.Set(posicion_suelo.x, posicion_suelo.y);

	//Propiedades para la definición del techo
	definicion_techo.type = b2_staticBody;
	definicion_techo.allowSleep = false;
	definicion_techo.position.Set(posicion_techo.x, posicion_techo.y);

	//RAGDOLL

	//Propiedades para la definición de la cabeza
	definicion_cabeza.type = b2_dynamicBody;
	definicion_cabeza.allowSleep = false;
	definicion_cabeza.position.Set(posicion_cabeza.x, posicion_cabeza.y);

	//Propiedades para la definición del cuerpo
	definicion_cuerpo.type = b2_dynamicBody;
	definicion_cuerpo.allowSleep = false;
	definicion_cuerpo.position.Set(posicion_cuerpo.x, posicion_cuerpo.y);

	//Propiedades para la definición del brazo derecho
	definicion_extremidades[0].type = b2_dynamicBody;
	definicion_extremidades[0].allowSleep = false;
	definicion_extremidades[0].position.Set(posicion_brazo_der.x, posicion_brazo_der.y);
	definicion_extremidades[0].angle = rotacion_brazo_der;

	//Propiedades para la definición del brazo izquierdo
	definicion_extremidades[1].type = b2_dynamicBody;
	definicion_extremidades[1].allowSleep = false;
	definicion_extremidades[1].position.Set(posicion_brazo_izq.x, posicion_brazo_izq.y);
	definicion_extremidades[1].angle = rotacion_brazo_izq;

	//Propiedades para la definición de la pierna derecha
	definicion_extremidades[2].type = b2_dynamicBody;
	definicion_extremidades[2].allowSleep = false;
	definicion_extremidades[2].position.Set(posicion_pierna_der.x, posicion_pierna_der.y);

	//Propiedades para la definición de la pierna izquierda
	definicion_extremidades[3].type = b2_dynamicBody;
	definicion_extremidades[3].allowSleep = false;
	definicion_extremidades[3].position.Set(posicion_pierna_izq.x, posicion_pierna_izq.y);

	///////////////////////////////////

	//CREACIÓN DE CUERPOS

	//Se crean los cuerpos en el mundo, usando las definiciones de cada uno.
	cuerpo_ragdoll[0] = mundo->CreateBody(&definicion_cabeza);
	cuerpo_ragdoll[1] = mundo->CreateBody(&definicion_cuerpo);
	cuerpo_ragdoll[2] = mundo->CreateBody(&definicion_extremidades[0]);
	cuerpo_ragdoll[3] = mundo->CreateBody(&definicion_extremidades[1]);
	cuerpo_ragdoll[4] = mundo->CreateBody(&definicion_extremidades[2]);
	cuerpo_ragdoll[5] = mundo->CreateBody(&definicion_extremidades[3]);

	cuerpo_suelo = mundo->CreateBody(&definicion_suelo);
	cuerpo_techo = mundo->CreateBody(&definicion_techo);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Crea los fijadores del proyecto.
void Simulacion::crearFijadores() {

	//DEFINICIONES FIJADORES

	//Definición del fijador de la cabeza
	definicion_fijador_cabeza.shape = &figuras[0];
	definicion_fijador_cabeza.density = 1.0f;
	definicion_fijador_cabeza.friction = 0.1f;

	//Definición del fijador del cuerpo
	definicion_fijador_cuerpo.shape = &figuras[1];
	definicion_fijador_cuerpo.density = 1.0f;
	definicion_fijador_cuerpo.friction = 0.2f;

	//Definición del fijador de extremidades
	definicion_fijador_extremidad.shape = &figuras[2];
	definicion_fijador_cuerpo.density = 1.0f;
	definicion_fijador_cuerpo.friction = 0.2f;

	//Definición del fijador del suelo
	definicion_fijador_suelo.shape = &figuras[3];
	definicion_fijador_suelo.friction = 0.2f;

	//Definición del fijador del techo
	definicion_fijador_techo.shape = &figuras[3];
	definicion_fijador_techo.friction = 0.2f;

	///////////////////////////////////

	//FIJADORES

	fijador_ragdoll[0] = cuerpo_ragdoll[0]->CreateFixture(&definicion_fijador_cabeza);
	fijador_ragdoll[1] = cuerpo_ragdoll[1]->CreateFixture(&definicion_fijador_cuerpo);

	for (int i = 2; i < 6; i++) {

		fijador_ragdoll[i] = cuerpo_ragdoll[i]->CreateFixture(&definicion_fijador_extremidad);

	}

	fijador_suelo = cuerpo_suelo->CreateFixture(&definicion_fijador_suelo);
	fijador_techo = cuerpo_techo->CreateFixture(&definicion_fijador_techo);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Crea las articulaciones para el proyecto.
void Simulacion::crearArticulaciones() {

	//ARTICULACIONES (JOINTS)
	
	//Definición del Enlace A: De CABEZA (A) a CUERPO (B)
	definicion_enlace_1.bodyA = cuerpo_ragdoll[0];
	definicion_enlace_1.bodyB = cuerpo_ragdoll[1];
	definicion_enlace_1.localAnchorA = {0.0f, 0.0f};
	definicion_enlace_1.localAnchorB = {0.0f, 0.0f};
	definicion_enlace_1.length = 0.01f;

	//Definición del Enlace B: De BRAZO DERECHO (A) a CUERPO (B)
	definicion_enlace_2.bodyA = cuerpo_ragdoll[2];
	definicion_enlace_2.bodyB = cuerpo_ragdoll[1];
	definicion_enlace_2.localAnchorA = { 0.0f, 0.0f };
	definicion_enlace_2.localAnchorB = { 0.0f, 0.0f };
	definicion_enlace_2.length = 0.01f;

	//Definición del Enlace C: De BRAZO IZQUIERDO (A) a CUERPO (B)

	//Definición del Enlace D: De PIERNA DERECHA (A) a CUERPO (B)

	//Definición del Enlace E: De PIERNA IZQUIERDA (A) a CUERPO (B)

	//Se crean todas las articulaciones
	articulaciones[0] = (b2DistanceJoint*)mundo->CreateJoint(&definicion_enlace_1);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Calcula la siguiente posición y velocidad de cada cuerpo físico.
void Simulacion::actualizarFisicas() {

	float tiempo_fisico = 1.0f / 60.0f;

	int iteraciones_velocidad = 6;
	int iteraciones_posicion = 2;

	mundo->Step(tiempo_fisico, iteraciones_velocidad, iteraciones_posicion);

}

void Simulacion::actualizarVisuales() {

	//Obtiene la posición y la rotación del cuerpo físico del ragdoll, y se la pasa al cuerpo visual
	float posicion_suelo_x = cuerpo_suelo->GetPosition().x;
	float posicion_suelo_y = cuerpo_suelo->GetPosition().y;

	float posicion_techo_x = cuerpo_techo->GetPosition().x;
	float posicion_techo_y = cuerpo_techo->GetPosition().y;

	visuales_suelo.setPosition(posicion_suelo_x * PIXELES_POR_METRO, posicion_suelo_y * PIXELES_POR_METRO);
	visuales_techo.setPosition(posicion_techo_x * PIXELES_POR_METRO, posicion_suelo_y * PIXELES_POR_METRO);

	for (int i = 0; i < 6; i++) {

		float posicion_x = cuerpo_ragdoll[i]->GetPosition().x;
		float posicion_y = cuerpo_ragdoll[i]->GetPosition().y;
		float rotacion = cuerpo_ragdoll[i]->GetAngle() * (b2_pi / 180.0f);

		visuales_ragdoll[i].setPosition(posicion_x * PIXELES_POR_METRO, posicion_y * PIXELES_POR_METRO);
		visuales_ragdoll[i].setRotation(rotacion);

	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Método encargado de procesar todos los inputs eventos del jugador sobre la ventana.
void Simulacion::procesarEventos() {

	while (ventana->pollEvent(*gestor_eventos)) {

		switch (gestor_eventos->type) {

		case Event::Closed:

			//Cierra la ventana
			ventana->close();

			break;

		case Event::KeyPressed:

			switch (gestor_eventos->key.code) {

			case Keyboard::Space:

				cout << "[INFO]: Tecla presionada: ESPACIO" << endl;

				//Aplica una fuerza vertical al muñeco para hacerlo saltar
				cuerpo_ragdoll[1]->ApplyForceToCenter(fuerzas_movimiento[0], true);

				break;

			case Keyboard::R:

				cout << "[INFO]: Tecla presionada: R" << endl;

				//Reinica la posición del ragdoll y cambia su velocidad a 0

				break;

			case Keyboard::D:

				cout << "[INFO]: Tecla presionada: D" << endl;

				//Aplica una fuerza horizontal (derecha) al muñeco
				cuerpo_ragdoll[1]->ApplyForceToCenter(fuerzas_movimiento[1], true);

				break;

			case Keyboard::A:

				cout << "[INFO]: Tecla presionada: A" << endl;

				//Aplica una fuerza horizontal (izquierda) al muñeco
				cuerpo_ragdoll[1]->ApplyForceToCenter(fuerzas_movimiento[2], true);

				break;
			}

			break;

		}

	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Método encargado de renderizar todo el contenido en pantalla.
void Simulacion::renderizar() {

	ventana->clear();

	
	//Aumentar el tamaño maximo del arreglo
	for (int i = 0; i < 6; i++) {

		ventana->draw(visuales_ragdoll[i]);

	}
	
	ventana->draw(visuales_suelo);
	ventana->draw(visuales_techo);

	ventana->display();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Método donde se llaman todas las actualizaciones y donde ocurre toda la lógica
void Simulacion::bucleSimulacion() {

	while (ventana->isOpen()) {

		procesarEventos();

		actualizarFisicas();

		actualizarVisuales();

		renderizar();

	}

}

//Destructor sin usar
Simulacion::~Simulacion() {}

