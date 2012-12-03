/*
*	Copyright (C) 2008-2012 Thorsten Liebig (Thorsten.Liebig@gmx.de)
*
*	This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU Lesser General Public License as published
*	by the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU Lesser General Public License for more details.
*
*	You should have received a copy of the GNU Lesser General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tinyxml.h"

#include "CSPropLorentzMaterial.h"

CSPropLorentzMaterial::CSPropLorentzMaterial(ParameterSet* paraSet) : CSPropDispersiveMaterial(paraSet) {Type=(CSProperties::PropertyType)(LORENTZMATERIAL | DISPERSIVEMATERIAL | MATERIAL);Init();}
CSPropLorentzMaterial::CSPropLorentzMaterial(CSProperties* prop) : CSPropDispersiveMaterial(prop) {Type=(CSProperties::PropertyType)(LORENTZMATERIAL | DISPERSIVEMATERIAL | MATERIAL);Init();}
CSPropLorentzMaterial::CSPropLorentzMaterial(unsigned int ID, ParameterSet* paraSet) : CSPropDispersiveMaterial(ID,paraSet) {Type=(CSProperties::PropertyType)(LORENTZMATERIAL | DISPERSIVEMATERIAL | MATERIAL);Init();}

CSPropLorentzMaterial::~CSPropLorentzMaterial()
{
	DeleteValues();
	m_Order = 0;
}

void CSPropLorentzMaterial::Init()
{
	m_Order = 0;
	EpsPlasma=NULL;
	MuePlasma=NULL;
	WeightEpsPlasma=NULL;
	WeightMuePlasma=NULL;
	EpsRelaxTime=NULL;
	MueRelaxTime=NULL;
	WeightEpsRelaxTime=NULL;
	WeightMueRelaxTime=NULL;
	InitValues();
	CSPropDispersiveMaterial::Init();
}

void CSPropLorentzMaterial::DeleteValues()
{
	for (int o=0;o<m_Order;++o)
	{
		delete[] EpsPlasma[o];
		delete[] MuePlasma[o];
		delete[] WeightEpsPlasma[o];
		delete[] WeightMuePlasma[o];
		delete[] EpsRelaxTime[o];
		delete[] MueRelaxTime[o];
		delete[] WeightEpsRelaxTime[o];
		delete[] WeightMueRelaxTime[o];
	}
	delete[] EpsPlasma;
	delete[] MuePlasma;
	delete[] WeightEpsPlasma;
	delete[] WeightMuePlasma;
	delete[] EpsRelaxTime;
	delete[] MueRelaxTime;
	delete[] WeightEpsRelaxTime;
	delete[] WeightMueRelaxTime;

	EpsPlasma=NULL;
	MuePlasma=NULL;
	WeightEpsPlasma=NULL;
	WeightMuePlasma=NULL;
	EpsRelaxTime=NULL;
	MueRelaxTime=NULL;
	WeightEpsRelaxTime=NULL;
	WeightMueRelaxTime=NULL;
}

void CSPropLorentzMaterial::InitValues()
{
//	DeleteValues();
	EpsPlasma=new ParameterScalar*[m_Order];
	MuePlasma=new ParameterScalar*[m_Order];
	WeightEpsPlasma=new ParameterScalar*[m_Order];
	WeightMuePlasma=new ParameterScalar*[m_Order];
	EpsRelaxTime=new ParameterScalar*[m_Order];
	MueRelaxTime=new ParameterScalar*[m_Order];
	WeightEpsRelaxTime=new ParameterScalar*[m_Order];
	WeightMueRelaxTime=new ParameterScalar*[m_Order];

	for (int o=0;o<m_Order;++o)
	{
		EpsPlasma[o] = new ParameterScalar[3];
		MuePlasma[o] = new ParameterScalar[3];
		WeightEpsPlasma[o] = new ParameterScalar[3];
		WeightMuePlasma[o] = new ParameterScalar[3];
		EpsRelaxTime[o] = new ParameterScalar[3];
		MueRelaxTime[o] = new ParameterScalar[3];
		WeightEpsRelaxTime[o] = new ParameterScalar[3];
		WeightMueRelaxTime[o] = new ParameterScalar[3];

		for (int n=0;n<3;++n)
		{
			EpsPlasma[o][n].SetValue(0);
			EpsPlasma[o][n].SetParameterSet(clParaSet);
			MuePlasma[o][n].SetValue(0);
			MuePlasma[o][n].SetParameterSet(clParaSet);
			WeightEpsPlasma[o][n].SetValue(1);
			WeightEpsPlasma[o][n].SetParameterSet(coordParaSet);
			WeightMuePlasma[o][n].SetValue(1);
			WeightMuePlasma[o][n].SetParameterSet(coordParaSet);
			EpsRelaxTime[o][n].SetValue(0);
			EpsRelaxTime[o][n].SetParameterSet(clParaSet);
			MueRelaxTime[o][n].SetValue(0);
			MueRelaxTime[o][n].SetParameterSet(clParaSet);
			WeightEpsRelaxTime[o][n].SetValue(1);
			WeightEpsRelaxTime[o][n].SetParameterSet(coordParaSet);
			WeightMueRelaxTime[o][n].SetValue(1);
			WeightMueRelaxTime[o][n].SetParameterSet(coordParaSet);
		}
	}
}


bool CSPropLorentzMaterial::Update(string *ErrStr)
{
	bool bOK=true;
	int EC=0;
	for (int o=0;o<m_Order;++o)
	{
		for (int n=0;n<3;++n)
		{
			EC=EpsPlasma[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property epsilon plasma frequency value (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}
			EC=MuePlasma[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property mue plasma frequency value (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}

			EC=WeightEpsPlasma[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property epsilon plasma frequency weighting function (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}
			EC=WeightMuePlasma[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property mue plasma frequency value weighting function (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}

			EC=EpsRelaxTime[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property epsilon relaxation time value (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}
			EC=MueRelaxTime[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property mue relaxation time value (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}

			EC=WeightEpsRelaxTime[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property epsilon relaxation time weighting function (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}
			EC=WeightMueRelaxTime[o][n].Evaluate();
			if (EC!=ParameterScalar::NO_ERROR) bOK=false;
			if ((EC!=ParameterScalar::NO_ERROR) && (ErrStr!=NULL))
			{
				stringstream stream;
				stream << endl << "Error in Lorentz Material-Property mue relaxation time value weighting function (ID: " << uiID << "): ";
				ErrStr->append(stream.str());
				PSErrorCode2Msg(EC,ErrStr);
			}
		}
	}
	return bOK & CSPropDispersiveMaterial::Update(ErrStr);
}

bool CSPropLorentzMaterial::Write2XML(TiXmlNode& root, bool parameterised, bool sparse)
{
	if (CSPropDispersiveMaterial::Write2XML(root,parameterised,sparse) == false) return false;
	TiXmlElement* prop=root.ToElement();
	if (prop==NULL) return false;

	string suffix;
	for (int o=0;o<m_Order;++o)
	{
		suffix = ConvertInt(o+1);
		TiXmlElement* value=prop->FirstChildElement("Property");
		if (value==NULL)
			return false;
		WriteVectorTerm(EpsPlasma[o],*value,"EpsilonPlasmaFrequency_"+suffix,parameterised);
		WriteVectorTerm(MuePlasma[o],*value,"MuePlasmaFrequency_"+suffix,parameterised);
		WriteVectorTerm(EpsRelaxTime[o],*value,"EpsilonRelaxTime_"+suffix,parameterised);
		WriteVectorTerm(MueRelaxTime[o],*value,"MueRelaxTime_"+suffix,parameterised);

		TiXmlElement* weight=prop->FirstChildElement("Weight");
		if (weight==NULL)
			return false;
		WriteVectorTerm(WeightEpsPlasma[o],*weight,"EpsilonPlasmaFrequency_"+suffix,parameterised);
		WriteVectorTerm(WeightMuePlasma[o],*weight,"MuePlasmaFrequency_"+suffix,parameterised);
		WriteVectorTerm(WeightEpsRelaxTime[o],*weight,"EpsilonRelaxTime_"+suffix,parameterised);
		WriteVectorTerm(WeightMueRelaxTime[o],*weight,"MueRelaxTime_"+suffix,parameterised);
	}
	return true;
}

bool CSPropLorentzMaterial::ReadFromXML(TiXmlNode &root)
{
	if (CSPropDispersiveMaterial::ReadFromXML(root)==false) return false;
	TiXmlElement* prop=root.ToElement();

	if (prop==NULL) return false;

	// count m_Order
	TiXmlElement* matProp=prop->FirstChildElement("Property");
	if (matProp!=NULL)
	{
		m_Order=1;
		while (1)
		{
			if (matProp->Attribute("EpsilonPlasmaFrequency_"+ConvertInt(m_Order+1)))
				++m_Order;
			else if (matProp->Attribute("MuePlasmaFrequency_"+ConvertInt(m_Order+1)))
				++m_Order;
			else
				break;
		}
	}
	else
		return false;

	InitValues();

	if (ReadVectorTerm(EpsPlasma[0],*matProp,"EpsilonPlasmaFrequency_1",0.0)==false)
		ReadVectorTerm(EpsPlasma[0],*matProp,"EpsilonPlasmaFrequency",0.0);
	if (ReadVectorTerm(MuePlasma[0],*matProp,"MuePlasmaFrequency_1",0.0)==false)
		ReadVectorTerm(MuePlasma[0],*matProp,"MuePlasmaFrequency",0.0);

	if (ReadVectorTerm(EpsRelaxTime[0],*matProp,"EpsilonRelaxTime_1",0.0)==false)
		ReadVectorTerm(EpsRelaxTime[0],*matProp,"EpsilonRelaxTime",0.0);
	if (ReadVectorTerm(MueRelaxTime[0],*matProp,"MueRelaxTime_1",0.0)==false)
		ReadVectorTerm(MueRelaxTime[0],*matProp,"MueRelaxTime",0.0);

	TiXmlElement* weightProp=prop->FirstChildElement("Weight");
	if (weightProp)
	{
		if (ReadVectorTerm(WeightEpsPlasma[0],*weightProp,"EpsilonPlasmaFrequency_1",1.0)==false)
			ReadVectorTerm(WeightEpsPlasma[0],*weightProp,"EpsilonPlasmaFrequency",1.0);
		if (ReadVectorTerm(WeightMuePlasma[0],*weightProp,"MuePlasmaFrequency_1",1.0)==false)
			ReadVectorTerm(WeightMuePlasma[0],*weightProp,"MuePlasmaFrequency",1.0);

		if (ReadVectorTerm(WeightEpsRelaxTime[0],*weightProp,"EpsilonRelaxTime_1",1.0)==false)
			ReadVectorTerm(WeightEpsRelaxTime[0],*weightProp,"EpsilonRelaxTime",1.0);
		if (ReadVectorTerm(WeightMueRelaxTime[0],*weightProp,"MueRelaxTime_1",1.0)==false)
			ReadVectorTerm(WeightMueRelaxTime[0],*weightProp,"MueRelaxTime",1.0);
	}

	for (int o=1;o<m_Order;++o)
	{
		ReadVectorTerm(EpsPlasma[o],*matProp,"EpsilonPlasmaFrequency_"+ConvertInt(o+1),0.0);
		ReadVectorTerm(MuePlasma[o],*matProp,"MuePlasmaFrequency_"+ConvertInt(o+1),0.0);

		ReadVectorTerm(EpsRelaxTime[o],*matProp,"EpsilonRelaxTime"+ConvertInt(o+1),0.0);
		ReadVectorTerm(MueRelaxTime[o],*matProp,"MueRelaxTime"+ConvertInt(o+1),0.0);

		if (weightProp)
		{
			ReadVectorTerm(WeightEpsPlasma[o],*weightProp,"EpsilonPlasmaFrequency_"+ConvertInt(o+1),1.0);
			ReadVectorTerm(WeightMuePlasma[o],*weightProp,"MuePlasmaFrequency_"+ConvertInt(o+1),1.0);

			ReadVectorTerm(WeightEpsRelaxTime[o],*weightProp,"EpsilonRelaxTime"+ConvertInt(o+1),1.0);
			ReadVectorTerm(WeightMueRelaxTime[o],*weightProp,"MueRelaxTime"+ConvertInt(o+1),1.0);
		}
	}

	return true;
}