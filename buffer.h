#ifndef BUFFER_H
#define BUFFER_H
#endif

/*
find . -name '*.cpp' | xargs wc -l
*/

//textur in main.cpp
/*
## Me
fasse mir mesh.h zusammen und erkläre mir was diese datei macht und wie sie es macht

## AI Assistant
# Zusammenfassung von mesh.h
Diese Header-Datei implementiert ein **3D-Mesh-System** für OpenGL, das 3D-Modelle aus benutzerdefinierten Binärdateien lädt und rendert.
## Hauptkomponenten
### 1. **Datenstrukturen für Materialien**
``` cpp
struct MDSMaterial  // Material-Eigenschaften
struct Material     // Material + Texturen (Diffuse & Normal Maps)
```
Diese Strukturen speichern:
- **Diffuse/Specular/Emissive** Farben (Phong-Beleuchtungsmodell)
- **Shininess** (Glanzgrad)
- **Texture IDs** für Diffuse- und Normal-Maps

### 2. **Mesh-Klasse**
Die -Klasse repräsentiert ein einzelnes 3D-Objekt: `Mesh`
**Konstruktor:**
- Nimmt Vertices, Indizes, Material und Shader entgegen
- Erstellt OpenGL-Buffer (, ) `VertexBuffer``IndexBuffer`
- Holt Uniform-Locations aus dem Shader für Materialeigenschaften

**render()-Methode:**
- Bindet Shader und Buffer
- Setzt Material-Uniforms (diffuse, specular, emissive, shininess)
- Bindet die Diffuse-Texture an Texture-Unit 0
- Rendert das Mesh mit `glDrawElements()`

### 3. **ModelRead-Klasse**
Die zentrale Klasse zum **Laden von 3D-Modellen aus Binärdateien**:
**Konstruktor - Datei-Format:**
1. **Materialien laden:**
    - Liest Anzahl der Materialien
    - Für jedes Material:
        - Liest MDSMaterial-Daten
        - Liest Diffuse- und Normal-Map-Dateinamen
        - Lädt Texturen mit `stb_image`
        - Erstellt OpenGL-Texturen mit Mipmapping
        - Verwendet für Diffuse (Gamma-Korrektur) `GL_SRGB8_ALPHA8`

2. **Meshes laden:**
    - Liest Anzahl der Meshes
    - Für jedes Mesh:
        - Liest Material-Index, Vertex- und Index-Anzahl
        - Liest alle Vertices (Position, Normal, Texturkoordinaten)
        - Liest alle Indizes
        - Erstellt -Objekt `Mesh`

**render()-Methode:**
- Rendert alle geladenen Meshes nacheinander

## Wie es funktioniert
1. **Laden:** liest ein proprietäres Binärformat und erstellt Mesh-Objekte `ModelRead`
2. **Vorbereitung:** Jedes kapselt OpenGL-Buffer und Material-Daten `Mesh`
3. **Rendering:** Bei werden alle Meshes mit ihren Materialien und Texturen gezeichnet `render()`

## Besonderheiten
- **Automatisches Mipmap-Generierung** für bessere Texture-Qualität
- **sRGB-Texturformat** für korrekte Gamma-Korrektur bei Diffuse-Maps
- **GLCALL-Makro** für OpenGL-Fehlerbehandlung
- **Resource Management** durch Destruktoren (RAII-Prinzip)

Diese Datei ist das Herzstück des Model-Loading-Systems und ermöglicht es, komplexe 3D-Modelle mit Materialien und Texturen effizient zu verwalten und zu rendern.


*/