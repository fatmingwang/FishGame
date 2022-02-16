#ifndef _RENDER_OBJECT_XML_PARSER_H_
#define _RENDER_OBJECT_XML_PARSER_H_
namespace FATMING_CORE
{
	class	cImageButton;
	cRenderObjectBehavior*GetRenderObjectBehavior(TiXmlElement*e_pElement);

//<cClickMouseBehaviorList Name="aabb">
//	<cImageButton Name="TestButton" PI="Image/Dice/Dice.pi" Image="Call" Pos="" />
//	<cImageButton Name="TestButton" MPDIList="Image/Dice/Dice.mpdi" MPDI="" />
//</cClickMouseBehaviorList>
	cClickMouseBehaviorList<cImageButton>*	GetClickMouseBehaviorList(TiXmlElement*e_pElement);
//end namespace FATMING_CORE
}
//end _RENDER_OBJECT_XML_PARSER_H_
#endif