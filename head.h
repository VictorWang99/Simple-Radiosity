#ifndef HEAD_H
#define HEAD_H
#include<math.h>
#include<iostream>
#include<stdio.h>
using namespace std;

//封装的向量
class Cvector {
public:
	float x;
	float y;
	float z;
	Cvector() {};
	~Cvector() {};
	Cvector(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}
	void normal() {
		float length = sqrt(x * x + y * y + z * z);
		x /= length;
		y /= length;
		z /= length;
	}
	Cvector operator+(Cvector other) {
		return Cvector(x + other.x, y + other.y, z + other.z);
	}
	Cvector operator-(Cvector other) {
		return Cvector(x - other.x, y - other.y, z - other.z);
	}
	float dot_product(Cvector other) {
		float result = 0;
		result = x * other.x + y * other.y + z * other.z;
		return result;
	}
	Cvector operator*(float a) {
		return Cvector(x*a, y*a, z*a);
	}
	float get_distance(Cvector other) {
		float result = 0;
		result = (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y) + (z - other.z)*(z - other.z);
		return sqrt(result);
	}
};

//三元组，可以用于储存颜色和反射率
class tuple3f {
public:
	float r;
	float g;
	float b;
	tuple3f() {
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}
	tuple3f(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

//离散化的单元
typedef struct Patch {
	int x1, x2, y1, y2, z1, z2;
	tuple3f color;
	tuple3f reflectance;
	Cvector n_vector;
	Cvector get_middle_point() {
		Cvector p((x1 + x2) / 2, (y1 + y2) / 2, (z1 + z2) / 2);
		return p;
	}
}Patch;


//将平面离散化为patch
void Surface(int x1, int x2, int y1, int y2, int z1, int z2, tuple3f init_color, tuple3f reflectance, Cvector n_vector, Patch* patches, int& index) {
	if (z2 - z1 == 0) {
		for (int i = x1; i < x2; i++) {
			for (int j = y1; j < y2; j++) {
				patches[index].x1 = i;
				patches[index].x2 = i + 1;
				patches[index].y1 = j;
				patches[index].y2 = j + 1;
				patches[index].z1 = z1;
				patches[index].z2 = z2;
				patches[index].color = init_color;
				patches[index].reflectance = reflectance;
				patches[index].n_vector = n_vector;
				index++;
			}
		}
	}
	if (y2 - y1 == 0) {
		for (int i = x1; i < x2; i++) {
			for (int j = z1; j < z2; j++) {
				patches[index].x1 = i;
				patches[index].x2 = i + 1;
				patches[index].z1 = j;
				patches[index].z2 = j + 1;
				patches[index].y1 = y1;
				patches[index].y2 = y2;
				patches[index].color = init_color;
				patches[index].reflectance = reflectance;
				patches[index].n_vector = n_vector;
				index++;
			}
		}
	}
	if (x2 - x1 == 0) {
		for (int i = z1; i < z2; i++) {
			for (int j = y1; j < y2; j++) {
				patches[index].z1 = i;
				patches[index].z2 = i + 1;
				patches[index].y1 = j;
				patches[index].y2 = j + 1;
				patches[index].x1 = x1;
				patches[index].x2 = x2;
				patches[index].color = init_color;
				patches[index].reflectance = reflectance;
				patches[index].n_vector = n_vector;
				index++;
			}
		}
	}
}

//半立方体的小方块
class Humi_patch {
public:
	float x;
	float y;
	float z;
	Cvector n_vector;
	Humi_patch() {};
	Humi_patch(float n_x, float n_y, float n_z, Cvector v) {
		x = n_x;
		y = n_y;
		z = n_z;
		n_vector = v;
	}
};

//半立方体
class Humicube {
private:
	void make_humi_face(Cvector n_vector) {
		Cvector p = pat.get_middle_point();
		Cvector mid = p + n_vector;
		if (n_vector.x != 0) {
            float n_x = mid.x, n_y = mid.y - 0.95, n_z = mid.z - 0.95;
            for (int i = 0; i < 20; i++) {
                n_z = mid.z - 0.95;
                for (int j = 0; j < 20; j++) {
					if ((Cvector(n_x, n_y, n_z) - p).dot_product(pat.n_vector) < -0.00001) {
                        n_z += 0.1;
						continue;
					}
					humi_cube[humicube_num].x = n_x;
					humi_cube[humicube_num].y = n_y;
					humi_cube[humicube_num].z = n_z;
                    humi_cube[humicube_num].n_vector = n_vector;
					humicube_num++;
                    n_z += 0.1;
				}
                n_y += 0.1;
			}
		}
		if (n_vector.y != 0) {
            float n_x = mid.x - 0.95, n_y = mid.y, n_z = mid.z - 0.95;
            for (int i = 0; i < 20; i++) {
                n_z = mid.z - 0.95;
                for (int j = 0; j < 20; j++) {
					if ((Cvector(n_x, n_y, n_z) - p).dot_product(pat.n_vector) < -0.00001) {
                        n_z += 0.1;
						continue;
					}
					humi_cube[humicube_num].x = n_x;
					humi_cube[humicube_num].y = n_y;
					humi_cube[humicube_num].z = n_z;
                    humi_cube[humicube_num].n_vector = n_vector;
					humicube_num++;
                    n_z += 0.1;
				}
                n_x += 0.1;
			}
		}
		if (n_vector.z != 0) {
            float n_x = mid.x - 0.95, n_y = mid.y - 0.95, n_z =mid.z;
            for (int i = 0; i < 20; i++) {
                n_y = mid.y - 0.95;
                for (int j = 0; j < 20; j++) {
					if ((Cvector(n_x, n_y, n_z) - p).dot_product(pat.n_vector) < -0.0001) {
                        n_y += 0.1;
						continue;
					}
					humi_cube[humicube_num].x = n_x;
					humi_cube[humicube_num].y = n_y;
					humi_cube[humicube_num].z = n_z;
                    humi_cube[humicube_num].n_vector = n_vector;
					humicube_num++;
                    n_y += 0.1;
				}
                n_x += 0.1;
			}
		}
	}

public:
	Patch pat;
	Humi_patch *humi_cube;
	int humicube_num;
	Humicube() {
		humicube_num = 0;
        humi_cube = new Humi_patch[1200];
	};
	~Humicube() {
		delete[]humi_cube;
	};
	void make_humicube() {
		Cvector middle = pat.get_middle_point();
		int sign = 0;
        if (pat.n_vector.x != 0) {
			make_humi_face(pat.n_vector);
			make_humi_face(Cvector(0, 1, 0));
			make_humi_face(Cvector(0, -1, 0));
			make_humi_face(Cvector(0, 0, 1));
			make_humi_face(Cvector(0, 0, -1));
		}
        if (pat.n_vector.y != 0) {
			make_humi_face(pat.n_vector);
			make_humi_face(Cvector(1, 0, 0));
			make_humi_face(Cvector(-1, 0, 0));
			make_humi_face(Cvector(0, 0, 1));
			make_humi_face(Cvector(0, 0, -1));
		}
        if (pat.n_vector.z != 0) {
			make_humi_face(pat.n_vector);
			make_humi_face(Cvector(0, 1, 0));
			make_humi_face(Cvector(0, -1, 0));
			make_humi_face(Cvector(1, 0, 0));
			make_humi_face(Cvector(-1, 0, 0));
		}
	}
};

//发出的射线
class Ray {
public:
	Cvector p;
	Cvector d;
	Ray() {}
	Ray(Cvector n_p, Cvector n_d) {
		p = n_p;
		d = n_d;
	}
	~Ray() {}
	float is_interset(Patch pa) {
		Cvector B(pa.x1, pa.y1, pa.z1);
		if (d.dot_product(pa.n_vector) < 0.00001 && d.dot_product(pa.n_vector) > -0.00001) return 0;
		float t = ((B - p).dot_product(pa.n_vector)) / (d.dot_product(pa.n_vector));
		if (t < 0) return 0;
		Cvector e = p + d * t;
		if (e.x >= pa.x1 && e.x <= pa.x2 && e.y >= pa.y1 && e.y <= pa.y2 &&e.z >= pa.z1 &&e.z <= pa.z2) {
			return t;
		}
		return 0;
	}
};


static float F[20000][20000];
void init_F() {
    for (int i = 0; i < 20000; i++) {
        for (int j = 0; j < 20000; j++) {
			F[i][j] = 0;
		}
	}
}

//计算patch间能量传输比例
void caculate_F(Patch* b, int b_num) {
    for (int n = 0; n < b_num; n++) {
		Humicube humi;
		humi.pat = b[n];
        humi.make_humicube();

        float Pi = 3.14159;
		for (int i = 0; i < humi.humicube_num; i++) {	
			Cvector humi_mid(humi.humi_cube[i].x, humi.humi_cube[i].y, humi.humi_cube[i].z);
			Cvector direction = humi_mid - b[n].get_middle_point();
			direction.normal();
			Ray r(b[n].get_middle_point(), direction);
			float t = b[n].get_middle_point().get_distance(humi_mid);
			float cosA = b[n].n_vector.dot_product(direction);
			float cosB = humi.humi_cube[i].n_vector.dot_product(direction);
			float p_f = cosA * cosB / Pi / t / t;	
            float min_d = 10000;
			int min_id = 0;
			for (int j = 0; j < b_num; j++) {
                if (r.is_interset(b[j])>0.001) {
					if (r.is_interset(b[j]) < min_d) {
						min_d = r.is_interset(b[j]);
						min_id = j;
					}
				}
			}
            if(min_d==10000){
                continue;
            }
			F[n][min_id] += p_f;
		}
	}

    for(int i=0;i<b_num;i++){
        float total_f=0;
        for(int j=0;j<b_num;j++){
            total_f+=F[i][j];
        }
        for(int j=0;j<b_num;j++){
            F[i][j]=F[i][j]/50;
        }
    }

}

//迭代求解方程组
void Southwell(Patch* patches,int patch_num){
    float e[20000];
    float b[20000];
    //red
    for(int i=0;i<patch_num;i++){
        e[i]=patches[i].color.r;
        b[i]=0;
    }
    while(1){
        float max_e=0;
        int index=0;
        for(int i=0;i<patch_num;i++){
            if(fabs(e[i])>max_e){
                index=i;
                max_e=fabs(e[i]);
            }
        }
        if(max_e<0.01){
            for(int i;i<patch_num;i++){
                patches[i].color.r=b[i];
            }
            break;
        }
        b[index] += e[index]/(1-patches[index].reflectance.r*F[index][index]);
        float temp=e[index];
        for(int i=0;i<patch_num;i++){
            e[i] += patches[i].reflectance.r*F[i][index]*temp/(1-patches[index].reflectance.r*F[index][index]);
        }
        e[index]= 0;
    }
    //green
    for(int i=0;i<patch_num;i++){
        b[i]=0;
        e[i]=patches[i].color.g;
    }
    while(1){
        float max_e=0;
        int index=0;
        for(int i=0;i<patch_num;i++){
            if(fabs(e[i])>max_e){
                index=i;
                max_e=fabs(e[i]);
            }
        }
        if(max_e<0.0185){
            for(int i;i<patch_num;i++){
                patches[i].color.g=b[i];
            }
            break;
        }
        b[index] += e[index]/(1-patches[index].reflectance.g*F[index][index]);
        float temp=e[index];
        for(int i=0;i<patch_num;i++){
            e[i] += patches[i].reflectance.g*F[i][index]*temp/(1-patches[index].reflectance.g*F[index][index]);
        }
        e[index]= 0;
    }
    //blue
    for(int i=0;i<patch_num;i++){
        e[i]=patches[i].color.b;
        b[i]=0;
    }
    while(1){
        float max_e=0;
        int index=0;
        for(int i=0;i<patch_num;i++){
            if(fabs(e[i])>max_e){
                index=i;
                max_e=fabs(e[i]);
            }
        }
        if(max_e<0.01){
            for(int i;i<patch_num;i++){
                patches[i].color.b=b[i];
            }
            break;
        }
        b[index] += e[index]/(1-patches[index].reflectance.b*F[index][index]);
        float temp=e[index];
        for(int i=0;i<patch_num;i++){
            e[i] += patches[i].reflectance.b*F[i][index]*temp/(1-patches[index].reflectance.b*F[index][index]);
        }
        e[index]= 0;
    }
}


//均值滤波器
void avg_filter(int begin,int x_len,int y_len,int z_len,Patch* patches){
    int num1=0,num2=0;
    if(z_len==0){
        num1=x_len;
        num2=y_len;
    }
    if(y_len==0){
        num1=x_len;
        num2=z_len;
    }
    if(x_len==0){
        num1=z_len;
        num2=y_len;
    }
    for(int i=0;i<num1-2;i++){
        for(int j=0;j<num2-2;j++){
            int index=begin+num2*i+j;
            float sum_r=0;
            float sum_g=0;
            float sum_b=0;
            for(int k=0;k<3;k++){
                sum_r+=patches[index+k*num2].color.r+patches[index+k*num2+1].color.r+patches[index+k*num2+2].color.r;
                sum_g+=patches[index+k*num2].color.g+patches[index+k*num2+1].color.g+patches[index+k*num2+2].color.g;
                sum_b+=patches[index+k*num2].color.b+patches[index+k*num2+1].color.b+patches[index+k*num2+2].color.b;
            }
            patches[index+num2+1].color.r=sum_r/9;
            patches[index+num2+1].color.g=sum_g/9;
            patches[index+num2+1].color.b=sum_b/9;

        }
    }
}

void store_F(){
    FILE * pFile;
    if((pFile = fopen ("E://CG/F.txt", "wb+"))==NULL)
    {
        printf("cant open the file");
        exit(0);
    }
    for(int i=0;i<20000;i++){
        fwrite (F+i , sizeof(float), 20000, pFile);
    }
    fclose (pFile);
}

void load_F(){
    FILE * pFile;
    if((pFile = fopen ("E://CG/F.txt", "rb"))==NULL)
    {
        printf("cant open the file");
        exit(0);
    }
    for(int i=0;i<20000;i++){
        fread (F+i , sizeof(float), 20000, pFile);
    }
    fclose (pFile);
}

#endif // HEAD_H
