#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Simulacion {

private:

	//Variables básicas
	int ventana_x;
	int ventana_y;
	string nombre_programa;

	//Escala pixeles-metros
	const int PIXELES_POR_METRO = 100;

	//////////////////
	//Variables SFML//
	//////////////////

	//Se crea la ventana
	RenderWindow* ventana;

	//Se crean los visuales del ragdoll y el suelo
	RectangleShape visuales_ragdoll[6];
	RectangleShape visuales_suelo;
	RectangleShape visuales_techo;

	//Gestor de eventos
	Event* gestor_eventos;

	//Crea variables de tamaño en metros
	Vector2f dimensiones_cabeza_m;
	Vector2f dimensiones_cuerpo_m;
	Vector2f dimensiones_extremidad_m;
	Vector2f dimensiones_suelo_m;

	//Crea variables de tamaño en pixeles
	Vector2f dimensiones_cabeza_p;
	Vector2f dimensiones_cuerpo_p;
	Vector2f dimensiones_extremidad_p;
	Vector2f dimensiones_suelo_p;

	///////////////////
	//Variables Box2D//
	///////////////////

	//Mundo físico y gravedad de Box2D
	b2World* mundo;
	b2Vec2 gravedad;

	//Definiciones para cada parte del ragdoll
	b2BodyDef definicion_cabeza;
	b2BodyDef definicion_cuerpo;
	b2BodyDef definicion_extremidades[4];
	b2BodyDef definicion_suelo;
	b2BodyDef definicion_techo;

	//Cuerpos creados a partir de las definiciones
	b2Body* cuerpo_ragdoll[6];
	b2Body* cuerpo_suelo;
	b2Body* cuerpo_techo;

	//Define un arreglo de figuras para cada parte
	b2PolygonShape figuras[4];

	//Definiciones de los fijadores
	b2FixtureDef definicion_fijador_cabeza;
	b2FixtureDef definicion_fijador_cuerpo;
	b2FixtureDef definicion_fijador_extremidad;
	b2FixtureDef definicion_fijador_suelo;
	b2FixtureDef definicion_fijador_techo;

	//Fijadores creados usando las definiciones
	b2Fixture* fijador_ragdoll[6];
	b2Fixture* fijador_suelo;
	b2Fixture* fijador_techo;

	//Definiciones para los enlaces (joints)
	b2DistanceJointDef definicion_enlace_1;
	b2DistanceJointDef definicion_enlace_2;
	b2DistanceJointDef definicion_enlace_3;
	b2DistanceJointDef definicion_enlace_4;
	b2DistanceJointDef definicion_enlace_5;

	//Enlaces creados a partir de las definiciones
	b2DistanceJoint* articulaciones[5];

	b2Vec2 fuerzas_movimiento[3];

public:

	Simulacion();
	void bucleSimulacion();
	void crearVisuales();
	void crearFisicas();
	void crearCuerpos();
	void crearFijadores();
	void crearArticulaciones();
	void actualizarFisicas();
	void actualizarVisuales();
	void procesarEventos();
	void renderizar();
	~Simulacion();

};

