/**
	A simple rope connecting drawbridge and gaffs
*/

public func ConnectPoints(int x1, int y1, int x2, int y2)
{
	var angle = Angle(x1, y1, x2, y2);
	var length = 880 * Distance(x1, y1, x2, y2) / 100;

	SetPosition(x1 + (x2-x1)/2, y1 + (y2-y1)/2);
	SetR(angle);
	SetObjDrawTransform(1000, 0, 0, 0, length, 0);
}