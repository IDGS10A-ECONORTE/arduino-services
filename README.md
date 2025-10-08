# 🌐 Proyecto IoT - Detección de Incendios y Sismos

Este repositorio contiene el desarrollo de un prototipo IoT diseñado para la **detección temprana de incendios y sismos**, con el objetivo de prevenir desastres y proporcionar alertas oportunas a través de una infraestructura conectada.

---

## 📌 Descripción General

El sistema se basa en una **placa Arduino** que recopila información de múltiples sensores ambientales y sísmicos.  
Estos datos son procesados localmente y enviados mediante el módulo **SIM800L GSM** a un servidor Flask, el cual se encarga de la ingestión y almacenamiento en una base de datos **SQL Server Express**.

---

## 🛠️ Componentes del Prototipo

- **Arduino Uno / Nano** (microcontrolador principal)
- **Módulo DTH11** (Temperatura y Humedad)
- **Módulo SIM800L GSM/GPRS** (conectividad móvil y envío de datos)  
- **Sensor MQ-135** (detección de gases y calidad del aire → detección de incendios)  
- **Sensor SW-420** (detección de vibraciones → detección de sismos)  
- **Pantalla LCD I2C** (visualización local de datos y estados)  
- **Módulo K026** (botón o sensor auxiliar de control)  
- **Zumbador Pasivo** (alertas acústicas locales)  
- **Módulo de carga TP4056** (gestión de batería recargable 18650)  
- **DC-DC Step-Up MT3608** (elevador de voltaje a 5V para alimentar Arduino)  
- **Batería 18650 de 3.7V**  

---

## 🔗 Conexión con Backend

El prototipo se conecta con un **servidor Flask** que recibe las peticiones HTTP enviadas por el Arduino a través del módulo GSM.  

### Flujo de Datos:
1. El Arduino recopila lecturas de los sensores.  
2. Envía un **HTTP POST** al servidor Flask.  
3. Flask procesa los datos y los guarda en una **base de datos SQL Server Express**.  
4. La información puede ser consumida por aplicaciones móviles/web para monitoreo en tiempo real.  

---

## 🖼️ Diagrama de Conexión

<img src="https://i.ibb.co/7d24dbS2/Esquema-de-conexion.jpg" alt="Diagrama del prototipo" width="500">
- Modulo DTH11 faltante en el diagrama, asignar un puerto digital libre

---

## 🚀 Tecnologías Usadas

- **Lenguaje Arduino (C/C++)** para el firmware del prototipo.  
- **Python + Flask** para la API de ingestión de datos.  
- **SQL Server Express** para almacenamiento de la información histórica.  
- **HTTP Protocol (POST Requests)** para la comunicación entre Arduino y Flask.  

---

## 📅 Funcionalidad de Envío de Datos

El sistema envía información en dos intervalos:  
- **Cada 10 minutos** → Actualizaciones periódicas.  
- **Cada 1 hora** → Registro histórico.  
