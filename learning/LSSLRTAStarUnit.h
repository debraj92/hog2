/*
 *  LSSLRTAStarUnit.h
 *  hog2
 *
 *  Created by Nathan Sturtevant on 10/6/09.
 *  Copyright 2009 NS Software. All rights reserved.
 *
 */

#ifndef LSSLRTAStarUnit_H
#define LSSLRTAStarUnit_H

#include "Unit.h"
#include <deque>
#include "FPUtil.h"
#include "LSSLRTAStar.h"

template <class state, class action, class environment>
class LSSLRTAStarUnit : public Unit<state, action, environment> {
	
public:
	LSSLRTAStarUnit(state &s, state &target, LSSLRTAStar<state, action, environment> *alg)
	{
		currentLoc = s;
		goalLoc = target;
		//goal = target;
		algorithm = alg;
		totalLearned = 0;
		StartNewTrial(0);
	}
	virtual ~LSSLRTAStarUnit() { delete algorithm; }
	const char *GetName() { return algorithm->GetName(); }
	bool Done() { return (currentLoc==goalLoc) && fequal(algorithm->GetAmountLearned(), totalLearned); }
	void StartNewTrial(StatCollection *s) {
		if (s)
		{
			s->AddStat("nodesExpanded", GetName(), nodesExpanded);
			s->AddStat("nodesTouched", GetName(), nodesTouched);
		}
		nodesExpanded = 0;
		nodesTouched = 0;
//		printf("LSSLRTA*: New trial: %f learned last trial; %f total\n",
//			   algorithm->GetAmountLearned()-totalLearned, algorithm->GetAmountLearned());
		totalLearned = algorithm->GetAmountLearned();
	}
	
	bool MakeMove(environment *, OccupancyInterface<state,action> *, SimulationInfo<state,action,environment> *, action& a);
	//  virtual void printRoundStats(FILE *f) { fprintf(f,"%8.2f",amountLearned); }
	
	void OpenGLDraw(const environment *e, const SimulationInfo<state,action,environment> *) const;
	void UpdateLocation(environment *, state &s, bool success, SimulationInfo<state,action,environment> *)
	{
		if (success) 
		{
			//std::cout << "Updating location from " << currentLoc << " to " << s << std::endl;
			currentLoc = s;
		}
		else {
			//std::cout << "Move failed, not updating location" << std::endl;
		}
	}
	void GetGoal(state &s) { s = goalLoc; }
	void GetLocation(state &s) { s = currentLoc; }
	virtual void LogFinalStats(StatCollection *s)
	{
		s->AddStat("nodesExpanded", GetName(), nodesExpanded);
		s->AddStat("nodesTouched", GetName(), nodesTouched);
		algorithm->LogFinalStats(s);
	}
private:
	long nodesExpanded, nodesTouched;
	LSSLRTAStar<state, action, environment> *algorithm;			// pointer to the LRTA* algorithm
	double totalLearned;			// Actual amount learned
	std::vector<state> path;
	state currentLoc, goalLoc;
	//Unit<state, action, environment> *goal;
};



template <class state, class action, class environment>
bool LSSLRTAStarUnit<state, action, environment>::MakeMove(environment *e, OccupancyInterface<state,action> *, SimulationInfo<state,action,environment> *, action& a)
{
	if (currentLoc == goalLoc)
		return false;
	//	if (GetUnitGroup() == 0)
	//		return false;
	if (path.size() <= 1)
	{
		algorithm->GetPath(e, currentLoc, goalLoc, path);
		nodesExpanded += algorithm->GetNodesExpanded();
		nodesTouched += algorithm->GetNodesTouched();
		if (path.size() <= 1)
			return false;
		//std::reverse(path.begin(), path.end());
	}
	a = e->GetAction(path[path.size()-1], path[path.size()-2]);
	path.pop_back();
	return true;
}

template <class state, class action, class environment>
void LSSLRTAStarUnit<state, action, environment>::OpenGLDraw(const environment *e, const SimulationInfo<state,action,environment> *si) const
{
	PublicUnitInfo<state, action, environment> i;
	si->GetPublicUnitInfo(si->GetCurrentUnit(), i);
	e->SetColor(0.2, 0.2, 1.0, 1.0);
	if (fgreater(si->GetSimulationTime(), i.nextTime))
		e->OpenGLDraw(i.currentState);
	else
		e->OpenGLDraw(i.lastState, i.currentState,
					  (si->GetSimulationTime()-i.lastTime)/(i.nextTime-i.lastTime));
	//e->OpenGLDraw(currentLoc);
	
	e->SetColor(0.0, 1.0, 0.0, 1.0);
	e->OpenGLDraw(goalLoc);
	e->SetColor(1.0, 1.0, 0.0, 1.0);
	e->OpenGLDraw(currentLoc);
	
	algorithm->OpenGLDraw(e);
	
	//	e->SetColor(0.0, 0.0, 0.5, 0.25);
	//	for (typename LSStateStorage::const_iterator it = hashTable.begin(); it != hashTable.end(); it++)
	//	{
	//		if ((*it).second.theState == currentLoc)
	//		{
	//		}
	//		else {
	//			e->OpenGLDraw((*it).second.theState);
	//		}
	//	}
}

#endif
