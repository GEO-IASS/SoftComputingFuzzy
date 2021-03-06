#include "stdafx.h"
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include "Fuzzy.h"


void Fuzzy::Fuzzification()
{
	for (std::list<IOType*>::iterator si = systemInputs.begin(); si != systemInputs.end(); ++si) {
		for (std::list<MFType*>::iterator mf = (*si)->membershipFunctions.begin(); mf != (*si)->membershipFunctions.end(); ++mf) {
			ComputeDegreeOfMembership(*mf,(*si)->value);
		}
	}
}

void Fuzzy::RuleEvaluation()
{
	float strength;

	for (std::list<RuleType*>::iterator rule = ruleBase.begin(); rule != ruleBase.end(); ++rule) {
		strength = UPPER_LIMIT;
		for (std::list <RuleElementType*>::iterator ip = (*rule)->ifSide.begin(); ip != (*rule)->ifSide.end(); ++ip) {
			strength = std::min(strength, *(*ip)->value);
		}
		for (std::list<RuleElementType*>::iterator tp = (*rule)->thenSide.begin(); tp != (*rule)->thenSide.end(); ++tp) {
			*((*tp)->value) = std::max(strength, *((*tp)->value));
		}
	}
}

void Fuzzy::Defuzzification()
{
	float sumOfProducts;
	float sumOfAreas;
	float area;
	float centroid;

	for (std::list<IOType*>::iterator so = systemOutputs.begin(); so != systemOutputs.end(); ++so) {
		sumOfProducts = 0;
		sumOfAreas = 0;

		for (std::list<MFType*>::iterator mf = (*so)->membershipFunctions.begin(); mf != (*so)->membershipFunctions.end(); ++mf) {
			area = ComputeAreaOfTrapezoid(*mf);
			centroid = (*mf)->point1 + ((*mf)->point2 - (*mf)->point1) / 2;
			sumOfProducts += area*centroid;
			sumOfAreas += area;
		}
		(*so)->value = sumOfProducts / sumOfAreas;
	}
}

void Fuzzy::ComputeDegreeOfMembership(MFType * _mf, float _input)
{
	float delta1;
	float delta2;
	delta1 = _input - _mf->point1;
	delta2 = _mf->point2 - _input;

	if ((delta1 <= 0) || (delta2 <= 0)) {
		_mf->value = 0;
	}
	else {
		_mf->value = std::min((_mf->slope1*delta1), (_mf->slope1*delta2));
		_mf->value = std::min(_mf->value,(float) UPPER_LIMIT);
	}
}

float Fuzzy::ComputeAreaOfTrapezoid(MFType * _mf)
{
	float run1;
	float run2;
	float base;
	float top;
	float area;
	base = _mf->point2 - _mf-> point1;
	run1 = _mf->value/_mf->slope1;
	run2 = _mf->value / _mf->slope2;
	top = base - run1 - run2;
	area = _mf->value * (base + top) / 2;
	return area;
}

void Fuzzy::Update()
{
	GetSystemInputs();
	Fuzzification();
	RuleEvaluation();
	Defuzzification();
	PutSystemOutputs();
}

Fuzzy::Fuzzy()
{
}


Fuzzy::~Fuzzy()
{
}
