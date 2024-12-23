#pragma once
//#include <Windows.h>
//#include "Window.h"
//#include "Device.h"
#include "Shaders.h"
//#include "Mesh.h"
//#include "geometry.h"
//#include "GamesEngineeringBase.h"
//#include "GEMLoader.h"
//#include "Texture.h"
//#include "GraphicMath.h"

Texture normalMapTexture1;

void loadTextures1(DXCore& dxcore) {
	//diffuseTexture.load(dxcore, "Textures/diffuse.png");
	normalMapTexture1.load(dxcore, "Textures/T-rex_Normal_OpenGL.png");
}

struct Bone
{
	std::string name;
	Matrix offset;
	int parentIndex;
};

struct Skeleton
{
	std::vector<Bone> bones;
	Matrix globalInverse;
};

struct AnimationFrame
{
	std::vector<Vec3> positions;
	std::vector<Quaternion> rotations;
	std::vector<Vec3> scales;
};

class AnimationSequence
{
public:
	std::vector<AnimationFrame> frames;
	float ticksPerSecond;
	Vec3 interpolate(Vec3 p1, Vec3 p2, float t) {
		return ((p1 * (1.0f - t)) + (p2 * t));
	}
	Quaternion interpolate(Quaternion q1, Quaternion q2, float t) {
		return Quaternion::Slerp(q1, q2, t);
	}
	float duration() {
		return ((float)frames.size() / ticksPerSecond);
	}

	void calcFrame(float t, int& frame, float& interpolationFact)
	{
		interpolationFact = t * ticksPerSecond;
		frame = (int)floorf(interpolationFact);
		interpolationFact = interpolationFact - (float)frame;
		frame = min(frame, frames.size() - 1);
	}

	int nextFrame(int frame)
	{
		return min(frame + 1, frames.size() - 1);
	}

	Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex)
	{
		Matrix scale = Matrix::scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrame(baseFrame)].scales[boneIndex], interpolationFact));
		Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrame(baseFrame)].rotations[boneIndex], interpolationFact).toMatrix();
		Matrix translation = Matrix::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrame(baseFrame)].positions[boneIndex], interpolationFact));
		//Matrix local = translation * rotation * scale;

		//int nextFrameIndex = nextFrame(baseFrame);
		//Matrix temp;
		//// interpolate scale, rotation and translation
		//Matrix scale = temp.scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrameIndex].scales[boneIndex], interpolationFact));
		//Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrameIndex].rotations[boneIndex], interpolationFact).toMatrix();
		//Matrix translation = Matrix::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrameIndex].positions[boneIndex], interpolationFact));
		Matrix local = scale * rotation * translation;
		//Matrix local = scale * translation * rotation;

		if (skeleton->bones[boneIndex].parentIndex > -1)
		{
			//Matrix global = matrices[skeleton->bones[boneIndex].parentIndex] * local;
			Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex];
			return global;
		}
		return local;
	}



};


class Animation
{
public:
	std::map<std::string, AnimationSequence> animations;
	Skeleton skeleton;
	void calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
		animations[name].calcFrame(t, frame, interpolationFact);
	}
	Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
		return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
	}

	int bonesSize() {
		int boneSize = skeleton.bones.size();
		return boneSize;
	}

	void calcFinalTransforms(Matrix* matrices)
	{
		for (int i = 0; i < 44; i++) // this->bonesSize()
		{
			//matrices[i] = matrices[i] * skeleton.bones[i].offset * skeleton.globalInverse;
			//matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;
			matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse;
		}
	}



};


class AnimationInstance
{
public:
	Animation animation;
	std::string currentAnimation;
	float t;
	Matrix matrices[256];
	std::vector<mesh> meshes;
	std::vector<std::string> textureFilenames;
	AABB box;
	
	void resetAnimationTime()
	{
		t = 0;
	}
	bool animationFinished()
	{
		if (t > animation.animations[currentAnimation].duration())
		{
			return true;
		}
		return false;
	}

	void update(std::string name, float dt) {
		if (name == currentAnimation) {
			t += dt;
		}
		else {
			currentAnimation = name;  t = 0;
		}
		if (animationFinished() == true) { resetAnimationTime(); }
		int frame = 0;
		float interpolationFact = 0;
		animation.calcFrame(name, t, frame, interpolationFact);
		for (int i = 0; i < animation.bonesSize(); i++)
		{
			matrices[i] = animation.interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
		}
		animation.calcFinalTransforms(matrices);
	}

	void init(std::string filename, DXCore& core, TextureManager& textureManager) {
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		std::vector<Vec3> allVertices;
		loader.load(filename, gemmeshes, gemanimation);
		for (int i = 0; i < gemmeshes.size(); i++) {
			mesh mesh;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.push_back(v);
				// get all vertices to compute bounding box
				allVertices.push_back(v.pos);
			}
			mesh.init(vertices, gemmeshes[i].indices, core);
			// load texture
			textureFilenames.push_back(gemmeshes[i].material.find("diffuse").getValue());
			textureManager.load(core, gemmeshes[i].material.find("diffuse").getValue());
			meshes.push_back(mesh);
		}
		box = box.computeAABB(allVertices);

		// bones
		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation.skeleton.bones.push_back(bone);
		}

		// load in anim data
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
				{
					Vec3 p;
					Quaternion q;
					Vec3 s;
					memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation.animations.insert({ name, aseq });
		}

		// load normal map
		loadTextures1(core);
	}

	void draw(DXCore& core, Shader& shader, TextureManager& textureManager) {
		for (int i = 0; i < meshes.size(); i++)
		{
			shader.updateShaderResource(core, "tex", textureManager.find(textureFilenames[i]));
			shader.updateShaderResource(core, "normalMap", normalMapTexture1.srv);
			meshes[i].draw(core);
		}
	}

};


