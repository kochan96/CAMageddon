#include "ModelLoader.h"
#include <tinyxml2.h>

#include "BezierPatch.h"
#include "BSplinePatch.h"

namespace CAMageddon
{
	std::unordered_map<std::string, glm::vec3> points;
	std::unordered_map<std::string, glm::vec3> notAssignedPoints;

	glm::vec3 ReadVector(tinyxml2::XMLElement* Pos)
	{
		glm::vec3 pos;
		pos.x = Pos->FloatAttribute("X");
		pos.y = Pos->FloatAttribute("Y");
		pos.z = Pos->FloatAttribute("Z");
		return pos;
	}

	void LoadPoints(tinyxml2::XMLElement* sceneElement)
	{
		for (auto elem = sceneElement->FirstChildElement("Point"); elem != nullptr; elem = elem->NextSiblingElement("Point"))
		{
			std::string name = elem->Attribute("Name");
			glm::vec3 pos = ReadVector(elem->FirstChildElement("Position"));
			notAssignedPoints.insert({ name,pos });
			points.insert({ name,pos });
		}
	}

	glm::vec3 FindPointByName(std::string name)
	{
		auto it = points.find(name);
		notAssignedPoints.erase(name);
		return it->second;
	}

	std::shared_ptr<BezierPatch> LoadBezierPatch(tinyxml2::XMLElement* bezierPatchElement)
	{
		std::string name = bezierPatchElement->Attribute("Name");
		bool isCylinder = bezierPatchElement->Attribute("WrapDirection") != std::string("None");
		bool isRowWrapDirection = bezierPatchElement->Attribute("WrapDirection") == std::string("Row");
		bool showPolygon = bezierPatchElement->BoolAttribute("ShowControlPolygon");
		int uDivisionCount = bezierPatchElement->IntAttribute("ColumnSlices");
		int vDivisionCount = bezierPatchElement->IntAttribute("RowSlices");

		std::vector<glm::vec3> controlPoints;

		auto pointRefsElement = bezierPatchElement->FirstChildElement();
		int rowCount = 0;
		int columnCount = 0;
		for (auto elem = pointRefsElement->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement())
		{
			rowCount = std::max(rowCount, elem->IntAttribute("Row"));
			columnCount = std::max(columnCount, elem->IntAttribute("Column"));
		}

		rowCount++;
		columnCount++;

		controlPoints.resize(rowCount * columnCount);

		for (auto elem = pointRefsElement->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement())
		{
			int Row = elem->IntAttribute("Row");
			int Column = elem->IntAttribute("Column");
			auto point = FindPointByName(elem->Attribute("Name"));
			if (isRowWrapDirection)
				controlPoints[Column * rowCount + Row] = point;
			else
				controlPoints[Row * columnCount + Column] = point;
		}

		auto bezierPatch = BezierPatch::CreateBezierPatch(
			name,
			controlPoints,
			rowCount,
			columnCount,
			uDivisionCount,
			vDivisionCount,
			isCylinder,
			showPolygon);

		return bezierPatch;
	}

	std::shared_ptr<BSplinePatch> LoadBSplinePatch(tinyxml2::XMLElement* bSplinePatchElement)
	{
		std::string name = bSplinePatchElement->Attribute("Name");
		bool isCylinder = bSplinePatchElement->Attribute("WrapDirection") != std::string("None");
		bool isRowWrapDirection = bSplinePatchElement->Attribute("WrapDirection") == std::string("Row");
		bool showPolygon = bSplinePatchElement->BoolAttribute("ShowControlPolygon");
		int uDivisionCount = bSplinePatchElement->IntAttribute("ColumnSlices");
		int vDivisionCount = bSplinePatchElement->IntAttribute("RowSlices");

		std::vector<glm::vec3> controlPoints;
		auto pointRefsElement = bSplinePatchElement->FirstChildElement();
		int rowCount = 0;
		int columnCount = 0;
		for (auto elem = pointRefsElement->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement())
		{
			rowCount = std::max(rowCount, elem->IntAttribute("Row"));
			columnCount = std::max(columnCount, elem->IntAttribute("Column"));
		}

		rowCount++;
		columnCount++;
		controlPoints.resize(rowCount * columnCount);

		for (auto elem = pointRefsElement->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement())
		{
			int Row = elem->IntAttribute("Row");
			int Column = elem->IntAttribute("Column");
			auto point = FindPointByName(elem->Attribute("Name"));
			if (isRowWrapDirection)
				controlPoints[Column * rowCount + Row] = point;
			else
				controlPoints[Row * columnCount + Column] = point;
		}

		auto bSplinePatch = BSplinePatch::CreateBSplinePatch(
			name,
			controlPoints,
			rowCount,
			columnCount,
			uDivisionCount,
			vDivisionCount,
			isCylinder,
			showPolygon);

		return bSplinePatch;
	}

	std::vector<std::shared_ptr<SurfaceUV>> CAMageddon::ModelLoader::LoadModel(const std::string& filePath)
	{
		tinyxml2::XMLDocument doc;

		doc.LoadFile(filePath.c_str());
		auto sceneElement = doc.FirstChildElement("Scene");

		LoadPoints(sceneElement);

		std::vector<std::shared_ptr<SurfaceUV>> surfaces;

		for (auto elem = sceneElement->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement()) {

			std::string type = elem->Name();
			if (type == "Point") {
			}
			else if (type == "BezierC0") {
			}
			else if (type == "BezierC2") {
			}
			else if (type == "BezierInter") {
			}
			else if (type == "Torus") {
			}
			else if (type == "PatchC0")
			{
				surfaces.push_back(LoadBezierPatch(elem));
			}
			else if (type == "PatchC2")
			{
				surfaces.push_back(LoadBSplinePatch(elem));
			}
		}

		notAssignedPoints.clear();
		points.clear();

		return surfaces;
	}
}
