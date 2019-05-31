#pragma once

#include <vector>
#include <list>
#include <deque>
#include <memory>

#include "Operation.h"

namespace ami
{
	class Pattern;

	class PatternInvalidException : public std::runtime_error
	{
	public:
		PatternInvalidException(int round, int operation)
			:
			std::runtime_error("Round " + std::to_string(round) + ", Operation " + std::to_string(operation) + " failed"),
			m_round(round), 
			m_operation(operation)
		{}

		int round() { return m_round; }
		int operation() { return m_operation; }

	private:
		int m_round = -1;
		int m_operation = -1;
	};

	class RoundStitchesCountException : public std::runtime_error
	{
	public:
		RoundStitchesCountException(int round, int availableStitches, int requiredStitches)
			:
			std::runtime_error("Round " + std::to_string(round) + " requires " + std::to_string(requiredStitches) + " stitches, available: " + std::to_string(availableStitches)),
			m_round(round)
		{}

		int round() { return m_round; }

	private:
		int m_round = -1;
	};

	// Graph class
	class PatternGraph
	{
	public:
		struct NodeData
		{
			Operation::Type op;
		};

		struct Face
		{
			int ids[3];
		};

		struct Edge
		{
			int from = 0;
			int to = 0;
			float distance = 0.f;
		};

		struct Joint
		{
			int from = 0;
			int to = 0;
		};

		struct Node
		{
			int id = 0;

			int last;
			//int under;
			int next;

			NodeData data;
		};

		class NodeIterator
		{
		public:
			NodeIterator(std::vector<Node> & nodes, int id) : m_nodes(&nodes), id(id) {}

			NodeIterator last() { return NodeIterator(*m_nodes, node().last); }
			//NodeIterator under() { return NodeIterator(*m_nodes, node().under); }
			NodeIterator next() { return NodeIterator(*m_nodes, node().next); }

			Node & node() { return m_nodes->at(id); }

			int id;

			bool operator!=(NodeIterator & other)
			{
				return id != other.id;
			}

			bool operator>(NodeIterator & other)
			{
				return id > other.id;
			}

			bool operator<(NodeIterator & other)
			{
				return id < other.id;
			}

			void operator++()
			{
				++id;
			}

			void operator--()
			{
				--id;
			}

		private:
			std::vector<Node> * m_nodes;
		};

		PatternGraph();
		PatternGraph(const Pattern & pattern);

		PatternGraph join(const PatternGraph & graph);

		void append(const Pattern & pattern);

		NodeIterator addNode(const NodeData & data);

		void addOutline(const NodeIterator & it);

		bool popOutline(unsigned int n = 1);

		int getAvailableStitches() { return m_outline.size() - 1; }

		void addEdge(int from, int to, float distance);

		void addJoint(int from, int to);

		void addFace(int a, int b, int c);

		NodeIterator at(int id)
		{
			return NodeIterator(m_nodes, id);
		}

		NodeIterator front()
		{
			return NodeIterator(m_nodes, m_outline.front());
		}

		NodeIterator back()
		{
			return NodeIterator(m_nodes, m_outline.back());
		}

		NodeIterator get(int id)
		{
			return NodeIterator(m_nodes, id);
		}

		const std::vector<Node> & getNodes() const {
			return m_nodes;
		}
		const std::vector<Edge> & getEdges() const {
			return m_edges;
		}
		const std::vector<Joint> & getJoints() const {
			return m_joints;
		}
		const std::vector<Face> & getFaces() const {
			return m_faces;
		}
		const std::deque<int> & getOutline() const {
			return m_outline;
		}

	private:
		std::vector<Node> m_nodes;
		std::vector<Edge> m_edges;
		std::vector<Joint> m_joints;
		std::vector<Face> m_faces;
		std::deque<int> m_outline;
	};

	// Define graph operations
	class GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const = 0;
		virtual int consumedStitches() const = 0;
		static std::unique_ptr<GraphOperation> getGraphOperation(Operation::Type type);
	};

	class LoopOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 0; };
		// Inherited via GraphOperation
	};

	class ChainOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 0; };
	};

	class SingleCrochetOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 1; };
	};

	class IncreaseOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 0; };
	};

	class DecreaseOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 2; };
	};

	class MagicRingOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 0; };
	};

	class SlipStitchOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 1; };
	};

	class JoinOperation : public GraphOperation
	{
	public:
		JoinOperation(int from, int with) : from(from), with(with) {}

		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 1; };

		int from, with;
	};

	class FinishOffOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 0; };
	};

	class SkipOperation : public GraphOperation
	{
	public:
		virtual bool apply(PatternGraph & pattern) const override;
		virtual int consumedStitches() const override { return 1; };
	};
}
