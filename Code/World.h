#ifndef World_h
#define World_h


#include "Graphics.h"


namespace Framework
{
	enum : uint32
	{
		kNodeCamera			= 'CAMR',
		kNodeLight			= 'LITE',
		kNodeFog			= 'FOG ',
		kNodeOccluder		= 'OCCL',
		kNodeBone			= 'BONE',
		kNodeGeometry		= 'GEOM'
	};

	enum : uint32
	{
		kCameraFrustum		= 'FRUS',
		kCameraOrtho		= 'ORTH'
	};

	enum : uint32
	{
		kLightInfinite		= 'INFT',
		kLightPoint			= 'PONT'
	};

	enum : uint32
	{
		kGeometryMesh		= 'MESH',
		kGeometrySphere		= 'SPHR',
		kGeometryBox		= 'BOX ',
		kGeometryText		= 'TEXT'
	};


	class Controller;


	struct Particle
	{
		Point3D			position;
		Vector3D		velocity;
		ColorRgba		color;
		float			radius;
		float			life;
	};


	struct BoneWeight
	{
		int32				boneIndex;
		float				weight;
	};


	struct SkinData
	{
		int32				boneCount;
		BoneWeight			boneWeight[1];

		const SkinData *GetNextSkinData(void) const
		{
			return (reinterpret_cast<const SkinData *>(boneWeight + boneCount));
		}
	};


	class Node : public Transformable, public Tree<Node>
	{
		private:

			uint32			nodeType;

		public:

			String<>		nodeName;

			Transform4D		nodeTransform;
			Controller		*nodeController;

			Node(uint32 type);
			virtual ~Node();

			uint32 GetNodeType(void) const
			{
				return (nodeType);
			}

			virtual void UpdateNode(void);

			Node *FindNode(const char *name);
	};


	class CameraNode : public Node
	{
		private:

			uint32			cameraType;

		protected:

			CameraNode(uint32 type);

		public:

			~CameraNode();

			uint32 GetCameraType(void) const
			{
				return (cameraType);
			}

			virtual Matrix4D CalculateProjectionMatrix(void) const = 0;
	};


	class FrustumCamera : public CameraNode
	{
		public:

			float			projectionDistance;
			float			aspectRatio;
			float			nearDepth;

			Plane			worldFrustumPlane[5];

			FrustumCamera(float g, float s, float n);
			~FrustumCamera();

			void UpdateNode(void) override;

			Matrix4D CalculateProjectionMatrix(void) const override;

			bool SphereVisible(const Point3D& center, float radius) const;
			bool BoxVisible(const Transform4D& transform, const Vector3D& size) const;
	};


	class OrthoCamera : public CameraNode
	{
		public:

			float			orthoWidth;
			float			orthoHeight;

			OrthoCamera(float w, float h);
			~OrthoCamera();

			Matrix4D CalculateProjectionMatrix(void) const override;
	};


	class LightNode : public Node, public ListElement<LightNode>
	{
		private:

			uint32			lightType;

		protected:

			LightNode(uint32 type, const ColorRgba& color);

		public:

			ColorRgba		lightColor;

			~LightNode();

			uint32 GetLightType(void) const
			{
				return (lightType);
			}

			virtual bool LightVisible(const FrustumCamera *camera) const;
	};


	class InfiniteLight : public LightNode
	{
		public:

			InfiniteLight(const ColorRgba& color);
			~InfiniteLight();
	};


	class PointLight : public LightNode
	{
		public:

			float			lightRange;

			PointLight(const ColorRgba& color, float range);
			~PointLight();

			bool LightVisible(const FrustumCamera *camera) const override;

			bool SphereIlluminated(const Point3D& center, float radius) const;
			bool BoxIlluminated(const Transform4D& transform, const Vector3D& size) const;
	};


	class FogNode : public Node
	{
		public:

			FogNode();
			~FogNode();

			Plane GetFogPlane(void) const
			{
				return (GetInverseWorldTransform().row2);
			}
	};


	class OccluderNode : public Node, public ListElement<OccluderNode>
	{
		private:

			Vector3D		occluderSize;

			int32			planeCount;
			Plane			planeArray[9];

		public:

			OccluderNode(const Vector3D& size);
			~OccluderNode();

			void CalculateOccluderPlanes(const FrustumCamera *cameraNode);

			bool SphereOccluded(const Point3D& center, float radius) const;
			bool BoxOccluded(const Transform4D& transform, const Vector3D& size) const;
	};


	class BoneNode : public Node
	{
		public:

			BoneNode();
			~BoneNode();
	};


	class GeometryNode : public Node, public Renderable, public ListElement<GeometryNode>
	{
		private:

			uint32			geometryType;

		protected:

			void EstablishStandardVertexArray(void);

		public:

			GeometryNode(uint32 type);
			~GeometryNode();

			uint32 GetGeometryType(void) const
			{
				return (geometryType);
			}

			virtual void PrepareToRender(const Matrix4D& viewProjectionMatrix);

			virtual bool GeometryVisible(const FrustumCamera *camera) const;
			virtual bool GeometryOccluded(const OccluderNode *occluder) const;
			virtual bool GeometryIlluminated(const PointLight *light) const;
	};


	class MeshGeometry : public GeometryNode
	{
		public:

			int32			meshVertexCount;
			int32			meshTriangleCount;

			Vertex			*meshVertexArray;
			Triangle		*meshTriangleArray;

			MeshGeometry(int32 vertexCount, int32 triangleCount, Vertex *vertexArray, Triangle *triangleArray);
			~MeshGeometry();
	};


	class SphereGeometry : public GeometryNode
	{
		private:

			enum
			{
				xdiv = 32,
				zdiv = 16,
				kSphereVertexCount = (xdiv + 1) * (zdiv + 1) - 2,
				kSphereTriangleCount = xdiv * (zdiv - 1) * 2
			};

			float			sphereRadius;

			Vertex			sphereVertex[kSphereVertexCount];
			Triangle		sphereTriangle[kSphereTriangleCount];

		public:

			SphereGeometry(float radius);
			~SphereGeometry();

			bool GeometryVisible(const FrustumCamera *camera) const override;
			bool GeometryOccluded(const OccluderNode *occluder) const override;
			bool GeometryIlluminated(const PointLight *light) const override;
	};


	class BoxGeometry : public GeometryNode
	{
		private:

			Vector3D		boxSize;

			Vertex			boxVertex[24];
			Triangle		boxTriangle[12];

		public:

			BoxGeometry(const Vector3D& size);
			~BoxGeometry();

			bool GeometryVisible(const FrustumCamera *camera) const override;
			bool GeometryOccluded(const OccluderNode *occluder) const override;
			bool GeometryIlluminated(const PointLight *light) const override;
	};


	class TextGeometry : public GeometryNode
	{
		private:

			Font			*textFont;
			Color4U			textColor;
			float			fontSize;

		public:

			TextGeometry(Font *font, float size, const char *text, const Color4U& color);
			~TextGeometry();

			void SetText(const char *text);

			void PrepareToRender(const Matrix4D& viewProjectionMatrix) override;
	};


	class Controller : public ListElement<Controller>
	{
		protected:

			Node	*targetNode;

			Controller(Node *node);

		public:

			bool	updateFlag;			// Set this flag to cause the UpdateController() function to be called the next time the world is rendered.

			virtual ~Controller();

			virtual void PreprocessController(void);
			virtual void MoveController(void);
			virtual void UpdateController(void);
	};


	class LightController : public Controller
	{
		private:

			Bivector3D		revolutionAxis;

		public:

			LightController(LightNode *node);
			~LightController();

			void MoveController(void) override;
	};


	class SkinController : public Controller
	{
		public:

			Array<const BoneNode *>		boneNodeArray;
			Array<Transform4D>			inverseBindTransformArray;

			char						*skinDataStorage;
			Transform4D					*transformTable;

			SkinController(MeshGeometry *mesh);
			~SkinController();

			MeshGeometry *GetTargetNode(void) const
			{
				return (static_cast<MeshGeometry *>(targetNode));
			}

			const SkinData *GetSkinData(void) const
			{
				return (reinterpret_cast<SkinData *>(skinDataStorage));
			}

			void PreprocessController(void) override;
			void UpdateController(void) override;
	};


	class WorldManager
	{
		private:

			Node					*rootNode;
			FrustumCamera			*cameraNode;
			FogNode					*fogNode;
			List<LightNode>			lightList;
			List<OccluderNode>		occluderList;
			List<GeometryNode>		geometryList;
			List<Controller>		controllerList;

			ColorRgba				ambientColor;

			CameraNode				*overlayCameraNode;
			List<GeometryNode>		overlayGeometryList;

			static void ConstructNormalMap(const Color4U *heightMap, Color2S *normalMap, int32 width, int32 height, float scale);
			static void ConstructParallaxMap(const Color4U *heightMap, Color1S *parallaxMap, int32 width, int32 height, float scale);
			static void ConstructHorizonMap(const Color4U *heightMap, Color4U *horizonMap, int32 width, int32 height, float scale);

			void GenerateHorizonCube(Color4U* texel);
			//void BuildFloor(Program *ambientProgram, Program *lightProgram, Texture *horizonTexture, Texture *horizonCubeTexture);
			void BuildFloor(Program* ambientProgram, Program* lightProgram);

			//void BuildWalls(Program *ambientProgram, Program *lightProgram, Texture *diffuseTexture, Texture *normalTexture, Texture *horizonTexture, Texture *horizonCubeTexture);
			void BuildWalls(Program* ambientProgram, Program* lightProgram, Texture* diffuseTexture, Texture* normalTexture);
			void BuildTree(const Point3D& position);
			void BuildGoblin(const Point3D& position);

		public:

			WorldManager();
			~WorldManager();

			Node *GetRootNode(void) const
			{
				return (rootNode);
			}

			void SetOverlayCameraNode(CameraNode *node)
			{
				overlayCameraNode = node;
			}

			void AddOverlayGeometryNode(GeometryNode *node)
			{
				overlayGeometryList.AppendListElement(node);
			}

			void PreprocessWorld(void);
			void RenderWorld(int32 *ambientDrawCount, int32 *lightDrawCount, int32 *lightSourceCount);
			void RenderOverlay(void);

			void BuildWorld(void);
	};


	extern WorldManager *worldManager;
}


#endif
