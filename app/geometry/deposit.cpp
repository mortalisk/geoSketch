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



    Vector3 point = surfaceNode->getPointFromUv(flowFrom);

    int fromx = round((point.x()+5)*4);
    int fromy = round((point.z()+5)*4);
    if (depositing) {

        if ((*previousDeposit)[fromy][fromx] < 0.1)
            (*previousDeposit)[fromy][fromx] = 0.1;
    }

    exchange(previousDeposit, deposited, 1, 0, fromx, fromy);

    exchange(deposited, previousDeposit, 0, 1, fromx, fromy);

    deposited = previousDeposit;

    QVector<vertex> triangles;
    for (int y = 0;y< deposited->size()-1; y++) {
        for (int x = 0; x < (*deposited)[0].size()-1; x++) {
            float ad = (*deposited)[y][x];
            float bd = (*deposited)[y][x+1];
            float cd = (*deposited)[y+1][x];
            float dd = (*deposited)[y+1][x+1];
            Vector3 a(samples[y][x].x(), samples[y][x].y() + ad, samples[y][x].z());
            Vector3 b(samples[y][x+1].x(), samples[y][x+1].y() + bd, samples[y][x+1].z());
            Vector3 c(samples[y+1][x].x(), samples[y+1][x].y() + cd, samples[y+1][x].z());
            Vector3 d(samples[y+1][x+1].x(), samples[y+1][x+1].y() + dd, samples[y+1][x+1].z());
            Vector3 na = (c-a).cross(b-a);
            Vector3 nb = (a-b).cross(c-b);
            Vector3 nc = (d-c).cross(a-c);
            Vector3 nd = (b-d).cross(c-d);


            if (ad > 0.001 || bd > 0.001 || cd > 0.001) {
                triangles.push_back(vertex(a, na));
                triangles.push_back(vertex(b, nb));
                triangles.push_back(vertex(c, nc));
            }

            if (bd > 0.001 || dd > 0.001 || cd > 0.001) {
                triangles.push_back(vertex(b, nb));
                triangles.push_back(vertex(d, nd));
                triangles.push_back(vertex(c, nc));
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

void Deposit::exchange(QVector<QVector<float> > * previousDeposit, QVector<QVector<float> > * deposited, int xinc, int yinc, int fromx, int fromy) {
for (int y = 0; y < samples.size(); y++) {

    for (int x = 0; x < samples[0].size(); x++) {
        float t = samples[y][x].y();
        float d = (*previousDeposit)[y][x];
        // t = terrain, d == deposit
        float tr, tl, dr, dl;


        int rightx = x+xinc;
        int righty = y+yinc;
        int leftx = x-xinc;
        int lefty = y-yinc;
        int vectorRightx = rightx-fromx;
        int vectorRighty = righty-fromy;
        int vectorLeftx = leftx-fromx;
        int vectorLefty = lefty-fromy;
        float distRight =sqrt(vectorRightx*vectorRightx+vectorRighty*vectorRighty);
        float distLeft = sqrt(vectorLeftx*vectorLeftx+vectorLefty*vectorLefty);
        float flowRateLeft = 2+pow(distLeft,1.1);
        float flowRateRight = 2+pow(distRight, 1.1);
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

        diffLeft = clamp(diffLeft/flowRateLeft, -d/2, dl/2);
        diffRigth = clamp(diffRigth/flowRateRight, -d/2, dr/2);

        (*deposited)[y][x] = (*previousDeposit)[y][x] + diffLeft + diffRigth;

    }

}
}

void Deposit::repositionOnSurface(SurfaceNode &surfacenode) {
    samples.clear();

    int gridsize = 40;
    float cellsize = 10.0/gridsize - 0.01/gridsize;


    // must cast rays from above, start in corner
    Vector3 pos(-4.99,10,-4.99);
    Vector3 dirx(cellsize, 0, 0);
    Vector3 diry(0, 0, cellsize);

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


