#pragma once
#include "ami/GL/GLNode.h"

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

#include <glm/vec3.hpp>	
#include <glm/matrix.hpp>	
#include <vector>

namespace ami
{
	class GLMesh : protected QOpenGLFunctions
	{
	public:
		GLMesh();
		~GLMesh();

		void draw();
		void drawVertices();
		void drawWireframe();

		void clear();
		void addVertex(const glm::vec3 & vertex);
		void addVertices(const std::vector<glm::vec3> & vertices);
		void setVertices(const std::vector<glm::vec3>& vertices);
		void clearVertices();
		bool hasVertices() const { return !m_vertices.empty(); }
		unsigned int numVertices() const { return m_vertices.size(); }

		void addIndex(unsigned int index);
		void addIndices(const std::vector<unsigned int> & indices);
		void setIndices(const std::vector<unsigned int> & indices);
		void clearIndices();
		bool hasIndices() const { return !m_indices.empty(); }
		unsigned int numIndices() const { return m_indices.size(); }

		GLNode & node() { return m_node; }

	private:
		void syncGL();

		std::vector<glm::vec3> m_vertices;
		std::vector<unsigned int> m_indices;

		bool m_verticesDirty = false;
		bool m_indicesDirty = false;

		QOpenGLVertexArrayObject * m_verticesVao = new QOpenGLVertexArrayObject(nullptr);
		GLuint m_verticesVbo = 0;
		GLuint m_indicesVbo = 0;

		GLNode m_node = GLNode(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0));
	};
}