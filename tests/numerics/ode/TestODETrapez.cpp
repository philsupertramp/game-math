#include "../../Test.h"
#include <math/numerics.h>

class ODETrapezTestCase : public Test
{
    bool TestOdeTrapez() {
        auto ode = []([[maybe_unused]] double t, const Matrix<double>& y) {
            return Matrix<double>({ { y(1, 0) }, { -sin(y(0, 0)) } });
        };
        std::vector<double> tInterval = { 0.0, 15.0 };
        Matrix<double> y0             = { { 1.0, 0.5 } };
        ODEOption options             = { 0.10, 1e-18, 500, []([[maybe_unused]] double t, const Matrix<double>& y) {
                                 return Matrix<double>({ { 0.0, 1.0 }, { -cos(y(0, 0)), 0.0 } });
                             } };

        auto foo = ODESolver::odeTrapez(ode, tInterval, y0, options);

        auto yResult = foo.Y;
        auto tResult = foo.T;

        double tExpected[151] = {
            0,
            0.100000000000000,
            0.200000000000000,
            0.300000000000000,
            0.400000000000000,
            0.500000000000000,
            0.600000000000000,
            0.700000000000000,
            0.800000000000000,
            0.900000000000000,
            1.000000000000000,
            1.100000000000000,
            1.200000000000000,
            1.300000000000000,
            1.400000000000000,
            1.500000000000000,
            1.600000000000000,
            1.700000000000000,
            1.800000000000000,
            1.900000000000001,
            2.000000000000000,
            2.100000000000001,
            2.200000000000001,
            2.300000000000001,
            2.400000000000001,
            2.500000000000001,
            2.600000000000001,
            2.700000000000001,
            2.800000000000001,
            2.900000000000001,
            3.000000000000001,
            3.100000000000001,
            3.200000000000002,
            3.300000000000002,
            3.400000000000002,
            3.500000000000002,
            3.600000000000002,
            3.700000000000002,
            3.800000000000002,
            3.900000000000002,
            4.000000000000002,
            4.100000000000001,
            4.200000000000001,
            4.300000000000001,
            4.400000000000000,
            4.500000000000000,
            4.600000000000000,
            4.699999999999999,
            4.799999999999999,
            4.899999999999999,
            4.999999999999998,
            5.099999999999998,
            5.199999999999998,
            5.299999999999997,
            5.399999999999997,
            5.499999999999996,
            5.599999999999996,
            5.699999999999996,
            5.799999999999995,
            5.899999999999995,
            5.999999999999995,
            6.099999999999994,
            6.199999999999994,
            6.299999999999994,
            6.399999999999993,
            6.499999999999993,
            6.599999999999993,
            6.699999999999992,
            6.799999999999992,
            6.899999999999991,
            6.999999999999991,
            7.099999999999991,
            7.199999999999990,
            7.299999999999990,
            7.399999999999990,
            7.499999999999989,
            7.599999999999989,
            7.699999999999989,
            7.799999999999988,
            7.899999999999988,
            7.999999999999988,
            8.099999999999987,
            8.199999999999987,
            8.299999999999986,
            8.399999999999986,
            8.499999999999986,
            8.599999999999985,
            8.699999999999985,
            8.799999999999985,
            8.899999999999984,
            8.999999999999984,
            9.099999999999984,
            9.199999999999983,
            9.299999999999983,
            9.399999999999983,
            9.499999999999982,
            9.599999999999982,
            9.699999999999982,
            9.799999999999981,
            9.899999999999981,
            9.999999999999980,
            10.099999999999980,
            10.199999999999980,
            10.299999999999979,
            10.399999999999979,
            10.499999999999979,
            10.599999999999978,
            10.699999999999978,
            10.799999999999978,
            10.899999999999977,
            10.999999999999977,
            11.099999999999977,
            11.199999999999976,
            11.299999999999976,
            11.399999999999975,
            11.499999999999975,
            11.599999999999975,
            11.699999999999974,
            11.799999999999974,
            11.899999999999974,
            11.999999999999973,
            12.099999999999973,
            12.199999999999973,
            12.299999999999972,
            12.399999999999972,
            12.499999999999972,
            12.599999999999971,
            12.699999999999971,
            12.799999999999971,
            12.899999999999970,
            12.999999999999970,
            13.099999999999969,
            13.199999999999969,
            13.299999999999969,
            13.399999999999968,
            13.499999999999968,
            13.599999999999968,
            13.699999999999967,
            13.799999999999967,
            13.899999999999967,
            13.999999999999966,
            14.099999999999966,
            14.199999999999966,
            14.299999999999965,
            14.399999999999965,
            14.499999999999964,
            14.599999999999964,
            14.699999999999964,
            14.799999999999963,
            14.899999999999963,
            14.999999999999963,
        };
        Matrix<double> yExpected({
        { 1.000000000000000, 0.500000000000000 },   { 1.045733091923598, 0.414661838471952 },
        { 1.082827827371348, 0.327232870483061 },   { 1.111102425765584, 0.238259097401650 },
        { 1.130426379571269, 0.148219978712055 },   { 1.140714563222527, 0.057543694313105 },
        { 1.141923016357802, -0.033374631607615 },  { 1.134046637762063, -0.124152940307158 },
        { 1.117118911766360, -0.214401579606908 },  { 1.091213681206351, -0.303703031593269 },
        { 1.056448874270890, -0.391593107115961 },  { 1.012991980289295, -0.477544772515947 },
        { 0.961066947072634, -0.560955891817260 },  { 0.900962038947348, -0.641142270688460 },
        { 0.833038054798323, -0.717337412292042 },  { 0.757736170948148, -0.788700264711462 },
        { 0.675584563579145, -0.854331882668596 },  { 0.587202904781997, -0.913301293274373 },
        { 0.493303842872561, -0.964679944914335 },  { 0.394690695663547, -1.007582999265961 },
        { 0.292250818198799, -1.041214550028989 },  { 0.186944448732337, -1.064912839300244 },
        { 0.079789259603464, -1.078190943277225 },  { -0.028158708819449, -1.080768425181033 },
        { -0.135826640408195, -1.072590206593882 }, { -0.242147667139371, -1.053830328029645 },
        { -0.346083189869044, -1.024880126563821 }, { -0.446643311498077, -0.986322306016825 },
        { -0.542904128885993, -0.938894041741498 }, { -0.634020998608854, -0.883443352715723 },
        { -0.719237333462693, -0.820883344361060 }, { -0.797888930897095, -0.752148604326985 },
        { -0.869404220518625, -0.678157188103615 }, { -0.933301105055706, -0.599780502637998 },
        { -0.989181241478828, -0.517822225824453 }, { -1.036722671554753, -0.433006375694036 },
        { -1.075671684018120, -0.345973873573301 }, { -1.105834700709391, -0.257286460252120 },
        { -1.127070853397488, -0.167436593509833 }, { -1.139285778819201, -0.076861914924429 },
        { -1.142427021962693, 0.014037052054591 },  { -1.136481309711031, 0.104877192978651 },
        { -1.121473840215407, 0.195272196933817 },  { -1.097469624696009, 0.284812113454147 },
        { -1.064576811967615, 0.373044141113739 },  { -1.022951815354994, 0.459455791138670 },
        { -0.972805941619848, 0.543461683564248 },  { -0.914413090213957, 0.624395344553572 },
        { -0.848117951974412, 0.701507420237339 },  { -0.774343999401833, 0.773971631214246 },
        { -0.693600443424680, 0.840899488328803 },  { -0.606487258097366, 0.901364218217473 },
        { -0.513697372565657, 0.954433492416715 },  { -0.416015224485868, 0.999209469179055 },
        { -0.314311077209450, 1.034873476349309 },  { -0.209530823623565, 1.060731595368392 },
        { -0.102681408897996, 1.076256699142989 },  { 0.005187544881200, 1.081122376440931 },
        { 0.113004902366867, 1.075224773272409 },   { 0.219700624705420, 1.058689673498644 },
        { 0.324228304937319, 1.031863931139347 },   { 0.425586117850204, 0.995292327118352 },
        { 0.522834868117428, 0.949682678226130 },   { 0.615112164882929, 0.895863257083879 },
        { 0.701642183188923, 0.834737109036011 },   { 0.781740922025368, 0.767237667692890 },
        { 0.854817271316027, 0.694289318120286 },   { 0.920370510573584, 0.616775467030850 },
        { 0.977985058911852, 0.535515499734519 },   { 1.027323380726418, 0.451250936556802 },
        { 1.068117941194565, 0.364640272806144 },   { 1.100163026910724, 0.276261441517036 },
        { 1.123307126884830, 0.186620557965074 },   { 1.137446431298840, 0.096165530315123 },
        { 1.142519866652270, 0.005303176753480 },   { 1.138505955652095, -0.085581396756971 },
        { 1.125421671211069, -0.176104292063544 },  { 1.103323343953963, -0.265862253078585 },
        { 1.072309576298677, -0.354413100027136 },  { 1.032526006956025, -0.441258286825903 },
        { 0.984171651947892, -0.525828813336749 },  { 0.927506419253161, -0.607475840557876 },
        { 0.862859256069270, -0.685467423119952 },  { 0.790636249012703, -0.758992718011374 },
        { 0.711327874324538, -0.827174775751926 },  { 0.625514510125518, -0.889092508228491 },
        { 0.533869303165356, -0.943811630974747 },  { 0.437157554938741, -0.990423333557550 },
        { 0.336231975454614, -1.028088256124998 },  { 0.232023450762381, -1.056082237719659 },
        { 0.125527364023245, -1.073839497063047 },  { 0.017785956855659, -1.080988646288677 },
        { -0.090132344428032, -1.077377379385150 }, { -0.197155355000411, -1.063082832062432 },
        { -0.302229812504848, -1.038406318026303 }, { -0.404342783513617, -1.003853102149067 },
        { -0.502540423611791, -0.960099699814419 }, { -0.595943036343478, -0.907952554819329 },
        { -0.683755795660902, -0.848302631529155 }, { -0.765274948059850, -0.782080416449793 },
        { -0.839889727635321, -0.710215175059630 }, { -0.907080549915404, -0.633601270542024 },
        { -0.966414272014631, -0.553073171442521 }, { -1.017537414133123, -0.469389670927310 },
        { -1.060168245409832, -0.383226954606885 }, { -1.094088570562719, -0.295179548450843 },
        { -1.119135940274802, -0.205767845790815 }, { -1.135196872569657, -0.115450800106292 },
        { -1.142201532818910, -0.024642404878771 }, { -1.140120187505247, 0.066269311152036 },
        { -1.128961625518998, 0.156901928572950 },  { -1.108773629257685, 0.246857996653308 },
        { -1.079645471231402, 0.335705163872341 },  { -1.041712303800644, 0.422958184742822 },
        { -0.995161194098019, 0.508064009309668 },  { -0.940238429601309, 0.590391280624537 },
        { -0.877257583166019, 0.669225648081274 },  { -0.806607686694288, 0.743772281353338 },
        { -0.728760734381901, 0.813166764894400 },  { -0.644277641144812, 0.876495099847379 },
        { -0.553811745970754, 0.932822803633792 },  { -0.458109000831110, 0.981232099159073 },
        { -0.358004144670413, 1.020865024054873 },  { -0.254412436216329, 1.050969145026816 },
        { -0.148316895419447, 1.070941670910817 },  { -0.040751443623774, 1.080367365002638 },
        { 0.067219221758995, 1.079045942652747 },   { 0.174521801436119, 1.067005650889732 },
        { 0.280097150931146, 1.044501339010814 },   { 0.382922081007293, 1.011997262512125 },
        { 0.482028781612882, 0.970136749599648 },   { 0.576520736336499, 0.919702344872698 },
        { 0.665584397695616, 0.861570882309635 },   { 0.748496344223214, 0.796668048242330 },
        { 0.824626069598752, 0.725926459268434 },   { 0.893434907702739, 0.650250302811295 },
        { 0.954471843758227, 0.570488418298468 },   { 1.007367092763010, 0.487416561797202 },
        { 1.051824353697093, 0.401728656884450 },   { 1.087612594945523, 0.314036168084153 },
        { 1.114558120644854, 0.224874345902471 },   { 1.132537534896679, 0.134713939134023 },
        { 1.141472080848010, 0.043976979892596 },   { 1.141323697055322, -0.046944655746354 },
        { 1.132093009782106, -0.137669089717970 },  { 1.113819366598364, -0.227803773956887 },
        { 1.086582909550872, -0.316925366992942 },  { 1.050508578965532, -0.404561244713871 },
        { 1.005771825370916, -0.490173827178432 },  { 0.952605682890340, -0.573149022433101 },
        { 0.891308722569721, -0.652790183979270 },
        });
        //        std::cout << yResult << std::endl;
        AssertEqual(yExpected, yResult);
        for(unsigned long i = 0; i < yResult.rows(); i++) { AssertEqual(tExpected[i], tResult(i, 0)); }
        return true;
    }


public:
    void run() override { TestOdeTrapez(); }
};

int main() {
    ODETrapezTestCase().run();
    return 0;
}
