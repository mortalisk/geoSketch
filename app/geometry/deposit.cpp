#include "deposit.h"
#include "surface.h"
#include "float.h"

void Deposit::prepareForDrawing() {

    if (deposit1.size() == 0) return;

    QVector<QVector<float> > * deposited;
    QVector<QVector<float> > * previousDeposit;
    if (alternateDeposits) {
       deposited = &deposit1;
       previousDeposit = &deposit2;
    } else {
       deposited = &deposit2;
       previousDeposit = &deposit1;
    }
    //alternateDeposits = !alternateDeposits;



    if (depositing) {
        if ((*previousDeposit)[0][10] < 0.1)
            (*previousDeposit)[0][10] = 0.1;
    }

    exchange(previousDeposit, deposited, 1, 0);

    exchange(deposited, previousDeposit, 0, 1);

    deposited = previousDeposit;

    QVector<vertex> triangles;
    for (int y = 0;y< deposited->size()-1; y++) {
        for (int x = 0; x < (*deposited)[0].size()-1; x++) {
            float ad = (*deposited)[y][x];
            float bd = (*deposited)[y][x+1];
            float cd = (*deposited)[y+1][x];
            float dd = (*deposited)[y+1][x+1];
            vertex a = vertex(samples[y][x].x(), samples[y][x].y() + ad, samples[y][x].z());
            vertex b = vertex(samples[y][x+1].x(), samples[y][x+1].y() + bd, samples[y][x+1].z());
            vertex c = vertex(samples[y+1][x].x(), samples[y+1][x].y() + cd, samples[y+1][x].z());
            vertex d = vertex(samples[y+1][x+1].x(), samples[y+1][x+1].y() + dd, samples[y+1][x+1].z());

            if (!(a.y == FLT_MIN || b.y == FLT_MIN || c.y == FLT_MIN) || ad > 0.1 || bd > 0.1 || cd > 0.1) {
                triangles.push_back(a);
                triangles.push_back(b);
                triangles.push_back(c);
            }

            if (!(b.y == FLT_MIN || d.y == FLT_MIN || c.y == FLT_MIN) || bd > 0.1 || dd > 0.1 || cd < 0.1) {
                triangles.push_back(b);
                triangles.push_back(d);
                triangles.push_back(c);
            }

        }
    }

    QVector<Vector3> noOutline;
    noOutline.push_back(Vector3(samples[0][0].x(), 5.0, samples[0][0].z()));
    noOutline.push_back(Vector3(samples[0][samples[0].size()-1].x(), 5.0, samples[0][samples[0].size()-1].z()));
    noOutline.push_back(Vector3(samples[samples.size()-1][samples[0].size()-1].x(), 5.0, samples[samples.size()-1][samples[0].size()-1].z()));
    noOutline.push_back(Vector3(samples[samples.size()-1][0].x(), 5.0, samples[samples.size()-1][0].z()));
    shape = new Surface(triangles,noOutline, false);

}

void Deposit::doTransformSurface(QVector<QVector<Vector3> > &, float, float ) {
    return;
}

void Deposit::exchange(QVector<QVector<float> > * previousDeposit, QVector<QVector<float> > * deposited, int xinc, int yinc) {
for (int y = 0; y < samples.size(); y++) {

    for (int x = 0; x < samples[0].size(); x++) {
        float t = samples[y][x].y();
        float d = (*previousDeposit)[y][x];
        // t = terrain, d == deposit
        float tr, tl, dr, dl;
        if (y+yinc >= samples.size() || x+xinc>=samples[0].size()) {
            tr = t;
            dr = d;
        } else {
            tr = samples[y+yinc][x+xinc].y();
            dr = (*previousDeposit)[y+yinc][x+xinc];
        }

        if (x - xinc < 0 || y - yinc < 0) {
            tl = t;
            dl = d;
        } else {
            tl = samples[y-yinc][x-xinc].y();
            dl = (*previousDeposit)[y-yinc][x-xinc];

        }

        float diffLeft = (tl+dl) - (t+d);
        float diffRigth = (tr+dr) - (t+d);

        diffLeft = clamp(diffLeft/2, -d/2, dl/2);
        diffRigth = clamp(diffRigth/2, -d/2, dr/2);

        (*deposited)[y][x] = (*previousDeposit)[y][x] + diffLeft + diffRigth;

    }

}
}

void Deposit::repositionOnSurface(SurfaceNode &surfacenode) {
    samples.clear();

    int gridsize = 40;
    float cellsize = 10.0/gridsize;

    Vector3 pos = surfacenode.getPointFromUv(flowFrom);
    Vector3 flowTowards = surfacenode.getPointFromUv(flowFrom+direction);
    Vector3 dir(flowTowards.x()-pos.x(), 0, flowTowards.z()-pos.z());
    // must cast rays from above
    pos = Vector3(pos.x(), pos.y()+100, pos.z());
    Vector3 dirx(-dir.z(),0, dir.x());
    dirx = dirx.normalize() * cellsize;
    Vector3 diry(dir.x(), 0, dir.z());
    diry = diry.normalize() * cellsize;


    // start at upper left side of grid
    pos = pos -dirx * gridsize/2;

    // direction of ray
    Vector3 down(0,-1, 0);
    for (int i = 0; i<gridsize; i++) {
        samples.push_back(QVector<Vector3>());
        for (int j = 0; j < gridsize; j++) {
            // calculate each point in a grid by sending intersection rays from top
            QVector<Vector3> points = surfacenode.intersectionPoints(pos, down);
            if (points.size() > 0) {
                samples[i].push_back(points[0]);
            } else {
                samples[i].push_back(Vector3(pos.x(),FLT_MIN,pos.z()));
            }

            pos += dirx;
        }
        pos = pos -dirx*gridsize;
        pos += diry;
    }



    QVector<float> oneRow(gridsize, 0);

    deposit1.fill(oneRow, gridsize);
    deposit2.fill(oneRow, gridsize);


}


