/*=========================================================================

  Library   : Image Registration Toolkit (IRTK)
  Module    : $Id$
  Copyright : Imperial College, Department of Computing
              Visual Information Processing (VIP), 2008 onwards
  Date      : $Date$
  Version   : $Revision$
  Changes   : $Author$

=========================================================================*/

#ifdef __APPLE__
#include <OpenGl/gl.h>
#include <OpenGl/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <mirtk/Image.h>
#include <mirtk/LookupTable.h>

LookupTable::LookupTable(int minData, int maxData)
{
	lookupTable = new ColorRGBA[maxData - minData + 1] - minData;
	_minData = minData;
	_maxData = maxData;
  _minDisplay  = minData;
  _maxDisplay  = maxData;
  _mode = ColorMode_Luminance;
  this->Update();
}

LookupTable::~LookupTable()
{
  lookupTable += _minData;
  delete [] lookupTable;
}

void LookupTable::Update()
{
  switch (_mode) {
  case ColorMode_Red:
    this->SetColorModeToRed();
    break;
  case ColorMode_Green:
    this->SetColorModeToGreen();
    break;
  case ColorMode_Blue:
    this->SetColorModeToBlue();
    break;
  case ColorMode_Luminance:
    this->SetColorModeToLuminance();
    break;
  case ColorMode_InverseLuminance:
    this->SetColorModeToInverseLuminance();
    break;
  case ColorMode_Rainbow:
    this->SetColorModeToRainbow();
    break;
  case ColorMode_HotMetal:
    this->SetColorModeToHotMetal();
    break;
  case ColorMode_Jacobian:
    this->SetColorModeToJacobian();
    break;
  case ColorMode_JacobianExpansion:
    this->SetColorModeToJacobianExpansion();
    break;
  case ColorMode_JacobianContraction:
    this->SetColorModeToJacobianContraction();
    break;
  case ColorMode_Custom:
    break;
  default:
    std::cerr << "LookupTable::Update: Unknown color mode" << std::endl;
    exit(1);
  }
}

void LookupTable::Initialize(int minData, int maxData)
{
  lookupTable += _minData;
  delete [] lookupTable;
	lookupTable = new ColorRGBA[maxData - minData + 1] - minData;
  _minData    = minData;
  _maxData    = maxData;
  _minDisplay = minData;
  _maxDisplay = maxData;
  this->Update();
}

void LookupTable::SetColorModeToLuminance()
{
  int i;

  _mode = ColorMode_Luminance;
  for (i = _minData; i < _minDisplay; i++) { 
    lookupTable[i] = 0;
    lookupTable[i].a = 1;
  }
  for (i = _minDisplay; i <= _maxDisplay; i++) {
    lookupTable[i] = round(((i - _minDisplay) /
                            (double)(_maxDisplay - _minDisplay) * 255));
    lookupTable[i].a = 1;
  }
  for (i = _maxDisplay+1; i < _maxData; i++) {
    lookupTable[i] = 255;
    lookupTable[i].a = 1;
  }
}

void LookupTable::SetColorModeToInverseLuminance()
{
  int i;

  _mode = ColorMode_InverseLuminance;
  for (i = _minData; i < _minDisplay; i++) {
    lookupTable[i]   = 255;
    lookupTable[i].a = 1;

  }
  for (i = _minDisplay; i <= _maxDisplay; i++) {
    lookupTable[i] = round(((_maxDisplay - i) /
                            (double)(_maxDisplay - _minDisplay) * 255));
    lookupTable[i].a = 1;

  }
  for (i = _maxDisplay+1; i < _maxData; i++) {
    lookupTable[i]   = 0;
    lookupTable[i].a = 1;
  }
}

void LookupTable::SetColorModeToHotMetal()
{
#ifdef HAS_COLOR
  int i;

  _mode = ColorMode_HotMetal;
  for (i = _minData; i < _minDisplay; i++) {
    lookupTable[i].r = 0;
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;

  }
  for (i = _minDisplay; i <= _maxDisplay; i++) {
    lookupTable[i].r = 255;
    lookupTable[i].g = round(((i - _minDisplay) /
                              (double)(_maxDisplay - _minDisplay) * 255));
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
  for (i = _maxDisplay+1; i < _maxData; i++) {
    lookupTable[i].r = 255;
    lookupTable[i].g = 255;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::SetColorModeToJacobian()
{
#ifdef HAS_COLOR
  int i;

  _mode = ColorMode_Jacobian;
  for (i = _minData; i < ((_minDisplay < 100) ? _minDisplay : 100); i++) {
    lookupTable[i].HSVtoRGB(180.0 / 360.0, 1, 1);
    lookupTable[i].a = 1;
  }
  for (i = _minDisplay; i <= 100; i++) {
    lookupTable[i].HSVtoRGB(- (i - 100) / (double)(_minDisplay - 100) * 60.0 / 360.0 + 240.0 / 360.0, 1, 1);
    lookupTable[i].a = 1;
  }
  for (i = 100; i <= _maxDisplay; i++) {
    lookupTable[i].HSVtoRGB((i - 100) / (double)(_maxDisplay - 100) * 60.0 / 360.0, 1, 1);
    lookupTable[i].a = 1;
  }
  for (i = ((_maxDisplay > 100) ? _maxDisplay : 100); i < _maxData; i++) {
    lookupTable[i].HSVtoRGB(60.0 / 360.0, 1, 1);
    lookupTable[i].a = 1;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::SetColorModeToJacobianExpansion()
{
#ifdef HAS_COLOR
  int i, _min, _max;

  _mode = ColorMode_JacobianExpansion;
  _min = ((_minDisplay > 100) ? _minDisplay : 100);
  _max = ((_maxDisplay > 100) ? _maxDisplay : 100);
  for (i = _minData; i < _min; i++) {
    lookupTable[i].r = 0;
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 0;
  }
  for (i = _min; i <= _max; i++) {
    lookupTable[i].HSVtoRGB((i - _min) / (double)(_max - _min + 1) * 60.0 / 360.0, 1, 1);
    lookupTable[i].a = (i - _min) / (double)(_max - _min + 1);
  }
  for (i = _max+1; i < _maxData; i++) {
    lookupTable[i].HSVtoRGB(60.0 / 360.0, 1, 1);
    lookupTable[i].a = 1;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::SetColorModeToJacobianContraction()
{
#ifdef HAS_COLOR
  int i, _min, _max;

  _mode = ColorMode_JacobianContraction;
  _min = ((_minDisplay < 100) ? _minDisplay : 100);
  _max = ((_maxDisplay < 100) ? _maxDisplay : 100);
  for (i = _minData; i < _min; i++) {
    lookupTable[i].HSVtoRGB(180 / 360.0, 1, 1);
    lookupTable[i].a = 1;
  }
  for (i = _min; i <= _max; i++) {
    lookupTable[i].HSVtoRGB((i - _max) / (double)(_max - _min + 1) * 60.0 / 360.0 + 240.0 / 360.0, 1, 1);
    lookupTable[i].a = -(i - _max) / (double)(_max - _min + 1);
  }
  for (i = _max+1; i < _maxData; i++) {
    lookupTable[i].r = 0;
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 0;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::SetColorModeToRed()
{
#ifdef HAS_COLOR
  int i;

  _mode = ColorMode_Red;
  for (i = _minData; i < _minDisplay; i++) {
    lookupTable[i].r = 0;
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
  for (i = _minDisplay; i <= _maxDisplay; i++) {
    lookupTable[i].r = round(((i - _minDisplay) /
                              (double)(_maxDisplay - _minDisplay) * 255));
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
  for (i = _maxDisplay+1; i < _maxData; i++) {
    lookupTable[i].r = 255;
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::SetColorModeToGreen()
{
#ifdef HAS_COLOR
  int i;

  _mode = ColorMode_Green;
  for (i = _minData; i < _minDisplay; i++) {
    lookupTable[i].r = 0;
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
  for (i = _minDisplay; i <= _maxDisplay; i++) {
    lookupTable[i].g = round(((i - _minDisplay) /
                              (double)(_maxDisplay - _minDisplay) * 255));
    lookupTable[i].r = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
  for (i = _maxDisplay+1; i < _maxData; i++) {
    lookupTable[i].g = 255;
    lookupTable[i].r = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::SetColorModeToBlue()
{
#ifdef HAS_COLOR
  int i;

  _mode = ColorMode_Blue;
  for (i = _minData; i < _minDisplay; i++) {
    lookupTable[i].r = 0;
    lookupTable[i].g = 0;
    lookupTable[i].b = 0;
    lookupTable[i].a = 1;
  }
  for (i = _minDisplay; i <= _maxDisplay; i++) {
    lookupTable[i].b = round(((i - _minDisplay) /
                              (double)(_maxDisplay - _minDisplay) * 255));
    lookupTable[i].g = 0;
    lookupTable[i].r = 0;
    lookupTable[i].a = 1;
  }
  for (i = _maxDisplay+1; i < _maxData; i++) {
    lookupTable[i].b = 255;
    lookupTable[i].g = 0;
    lookupTable[i].r = 0;
    lookupTable[i].a = 1;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::SetColorModeToRainbow()
{
#ifdef HAS_COLOR
  int i;

  _mode = ColorMode_Rainbow;
  for (i = _minData; i < _minDisplay; i++) {
    lookupTable[i].HSVtoRGB(2.0/3.0, 1, 1);
    lookupTable[i].a = 1;
  }
  for (i = _minDisplay; i <= _maxDisplay; i++) {
    lookupTable[i].HSVtoRGB((_maxDisplay - i) / (double)(_maxDisplay - _minDisplay) * 2.0/3.0, 1, 1);
    lookupTable[i].a = 1;
  }
  for (i = _maxDisplay+1; i < _maxData; i++) {
    lookupTable[i].HSVtoRGB(0, 1, 1);
    lookupTable[i].a = 1;
  }
#else
  this->SetColorModeToLuminance();
#endif
}

void LookupTable::Read(char *filename)
{
  float a;
  char buffer[255];
  int i, r, g, b, value1, value2;

  // Open file
  std::ifstream from(filename);

  // Check file format
  from >> buffer;
  if (strcmp(buffer, "LookupTable") != 0) {
    std::cerr << "LookupTable::Read: Can't read lookup table" << std::endl;
    exit(1);
  }

  // Read _min and _max values
  from >> value1 >> value2;

  // Initialize lookup table
  this->Initialize(value1, value2);

  for (i = _minData; i <= _maxData; i++) {
    from >> r >> g >> b >> a;
    lookupTable[i].r = r;
    lookupTable[i].g = g;
    lookupTable[i].b = b;
    lookupTable[i].a = a;
  }

  // This is a custom lookup table
  _mode = ColorMode_Custom;
}

void LookupTable::Write(char *)
{
  std::cerr << "LookupTable::Write: Not yet implemented" << std::endl;
  exit(1);
}

