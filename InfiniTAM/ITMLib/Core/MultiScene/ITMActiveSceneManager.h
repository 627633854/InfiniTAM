// Copyright 2014 Isis Innovation Limited and the authors of InfiniTAM

#pragma once

#include "ITMMultiSceneManager.h"

namespace ITMLib
{
	/** \brief
	*/
	class ITMActiveSceneManager
	{
	public:
		typedef enum { PRIMARY_SCENE, NEW_SCENE, LOOP_CLOSURE, RELOCALISATION, LOST, LOST_NEW } SceneActivity;

	private:
		struct ActiveDataDescriptor {
			int sceneIndex;
			SceneActivity type;
			std::vector<Matrix4f> constraints;
			ORUtils::SE3Pose estimatedPose;
			int trackingAttempts;
		};

		ITMMultiSceneManager *localSceneManager;
		std::vector<ActiveDataDescriptor> activeData;

		int CheckSuccess_relocalisation(int dataID) const;
		int CheckSuccess_newlink(int dataID, int primaryDataID, int *inliers, ORUtils::SE3Pose *inlierPose) const;
		void AcceptNewLink(int dataId, int primaryDataId, const ORUtils::SE3Pose & pose, int weight);

		float visibleOriginalBlocks(int dataID) const;
		bool shouldStartNewArea(void) const;
		bool shouldMovePrimaryScene(int newDataIdx, int bestDataIdx, int primaryDataIdx) const;

	public:
		void initiateNewScene(bool isPrimaryScene = false);
		int initiateNewLink(int sceneID, const ORUtils::SE3Pose & pose, bool isRelocalisation);

		void recordTrackingResult(int dataID, int trackingSuccess, bool primaryTrackingSuccess);
		void maintainActiveData(void);

		int findPrimaryDataIdx(void) const;
		int findPrimarySceneIdx(void) const;

		int findBestVisualisationDataIdx(void) const;
		int findBestVisualisationSceneIdx(void) const;

		int numActiveScenes(void) const
		{ return static_cast<int>(activeData.size()); }
		int getSceneIndex(int dataIdx) const
		{ return activeData[dataIdx].sceneIndex; }
		SceneActivity getSceneType(int dataIdx) const
		{ return activeData[dataIdx].type; }

		ITMActiveSceneManager(ITMMultiSceneManager *localSceneManager);
		~ITMActiveSceneManager(void) {}
	};
}
